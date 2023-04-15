package myrpc

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"log"
	"myrpc/codec"
	"net"
	"sync"
)

// 一次rpc所需要的信息。
type Call struct {
	Seq           uint64
	ServiceMethod string
	Args          interface{}
	Reply         interface{}
	Error         error
	Done          chan *Call //每次调用结束就会把call传递给done，通过done.error判断是否有错
}

//这是为了支持异步调用添加的字段， 当一个调用结束的时候，会调用call.done() 通知调用方
func (call *Call) done() {
	call.Done <- call
}

type Client struct {
	cc       codec.Codec //编解码器
	opt      *Option
	sending  sync.Mutex       //为了保证请求的有序发送
	header   codec.Header     //每个请求的消息头，header只有在请求发送时才需要，而请求是互斥发送的，因此每个客户端只需要一个，声明在client结构体中可以复用
	mu       sync.Mutex       //
	seq      uint64           //发送的请求编号，每个请求拥有唯一编号
	pending  map[uint64]*Call //存储未处理完的请求， 键为编号，值为call实例
	closing  bool             //用户主动关闭，
	shutdown bool             //出现不可预知的错误导致的关闭
}

var _ io.Closer = (*Client)(nil)

var ErrShutdown = errors.New("connection is shut down")

func (client *Client) Close() error {
	client.mu.Lock()
	defer client.mu.Unlock()
	if client.closing {
		return ErrShutdown
	}
	client.closing = true
	return client.cc.Close()
}

func (client *Client) IsAvailable() bool {
	client.mu.Lock()
	defer client.mu.Unlock()
	return !client.shutdown && !client.closing
}

// 对于客户端来说，最重要的就是2个功能， 接收功能和发送功能，先实现接收功能
// 三种情况
//1.call不存在，可能是请求没有发送完整或因为其他原因被取消， 但是服务端仍旧处理了，2.call存在，但服务端出错，即h.Error不为空
//3. call存在，服务端正常处理， 那么需从body中读取reply的值

func (client *Client) receive() {
	var err error
	for err == nil {
		var h codec.Header
		if err = client.cc.ReadHeader(&h); err != nil {
			break
		}

		// 当接收了这次返回的结果就把这次call 从 client.pending中移除
		call := client.removeCall(h.Seq) // call是指针类型
		switch {
		case call == nil:
			//这种情况一般是写数据的时候失败或者call已经被取走了
			err = client.cc.ReadBody(nil)
		case h.Error != "":
			// 返回的头部字段error不为空，说明服务端在处理的过程中发生了错误
			call.Error = fmt.Errorf(h.Error)
			err = client.cc.ReadBody(nil)
			call.done()
		default:
			// 正常情况
			err = client.cc.ReadBody(call.Reply)
			if err != nil {
				call.Error = errors.New("reading body:" + err.Error())
			}
			call.done()
		}
	}
	// 循环退出则表明出现了错误，中断 client.pending
	client.terminateCalls(err)
}

//创建client实例时， 需要完成一开始的协议交换，即发送option给服务端，协商好消息的编解码发送
//再创建一个子协程调用receive接收响应

func NewClient(conn net.Conn, opt *Option) (*Client, error) {
	f := codec.NewCodecFuncMap[opt.CodecType]
	if f == nil {
		err := fmt.Errorf("错误的编码方式: %s", opt.CodecType)
		log.Println("rpc client: 编码方式错误:", err)
		return nil, err
	}

	//把编码方式发送给服务端
	if err := json.NewEncoder(conn).Encode(opt); err != nil {
		log.Println("rpc client : 发送option失败:", err)
		_ = conn.Close()
		return nil, err
	}
	return newClientCodec(f(conn), opt), nil
}

func newClientCodec(cc codec.Codec, opt *Option) *Client {
	client := &Client{
		seq:     1,
		cc:      cc,
		opt:     opt,
		pending: make(map[uint64]*Call),
	}
	// 在生成client对象的时候，偷偷起了一个协程去处理客户端对于网络事件的接收
	go client.receive()
	return client
}

func parseOptions(opts ...*Option) (*Option, error) {
	if len(opts) == 0 || opts[0] == nil {
		return DefaultOption, nil // 不设置就返回默认的gob方式
	}
	if len(opts) != 1 {
		return nil, errors.New("options 数量超过了1")
	}

	opt := opts[0]
	opt.MagicNumber = DefaultOption.MagicNumber
	if opt.CodecType == "" {
		opt.CodecType = DefaultOption.CodecType
	}
	return opt, nil
}

func Dial(network, address string, opts ...*Option) (client *Client, err error) {
	opt, err := parseOptions(opts...)
	if err != nil {
		return nil, err
	}
	conn, err := net.Dial(network, address) //真正的请求过程
	if err != nil {
		return nil, err
	}

	//如果conn 是nil 则关闭连接
	defer func() {
		if client == nil {
			_ = conn.Close()
		}
	}()
	return NewClient(conn, opt)
}

// 将参数call添加到 client.pending中， 并更新client.seq
func (client *Client) registerCall(call *Call) (uint64, error) {
	client.mu.Lock()
	defer client.mu.Unlock()
	if client.closing || client.shutdown {
		return 0, ErrShutdown
	}
	call.Seq = client.seq
	client.pending[call.Seq] = call
	client.seq++
	return call.Seq, nil
}

//根据 client.seq,  从client.pending中移除对应的call， 并返回
func (client *Client) removeCall(seq uint64) *Call {
	client.mu.Lock()
	defer client.mu.Unlock()
	call := client.pending[seq]
	delete(client.pending, seq)
	return call
}

//服务段或者客户端发生错误时调用， 将Client.shutdown设置为true，并且将错误信息通知所有pending状态的call
func (client *Client) terminateCalls(err error) {
	client.sending.Lock()
	defer client.sending.Unlock()
	client.mu.Lock()
	defer client.mu.Unlock()
	client.shutdown = true
	for _, call := range client.pending {
		call.Error = err
		call.done()
	}
}

//目前位置， rpc客户端已经具备了完整的创建连接和接收响应的能力了， 还需要实现发送请求的能力

func (client *Client) send(call *Call) {
	client.sending.Lock()
	defer client.sending.Unlock()

	//注册当前调用的这个 call
	seq, err := client.registerCall(call)
	if err != nil {
		call.Error = err
		call.done()
		return
	}

	//准备请求头部
	// 可以发现所有的call 都在复用client 这个结构体。
	client.header.ServiceMethod = call.ServiceMethod
	client.header.Seq = seq
	client.header.Error = ""

	//编码并且发送这次请求
	if err := client.cc.Write(&client.header, call.Args); err != nil {
		// 每完成一次发送就从 client.pending 移除一个 call
		// 这里是出错的情况移除， 但正常来讲是客户端在 .receive()中处理了数据后才移除
		call := client.removeCall(seq)
		if call != nil {
			call.Error = err
			call.done()
		}
	}
}

func (client *Client) Go(ServiceMethod string, args, reply interface{}, done chan *Call) *Call {
	if done == nil {
		done = make(chan *Call, 10)
	} else if cap(done) == 0 {
		log.Panic("rpc client: done 管道没有内容")
	}

	call := &Call{
		ServiceMethod: ServiceMethod, //" Foo.Sum "
		Args:          args,          //" myrpc 请求i "
		Reply:         reply,         //传递的是引用类型。
		Done:          done,
	}
	client.send(call)
	return call
}

func (client *Client) Call(serviceMethod string, args, reply interface{}) error {
	//serviceMethod 为  "Foo.Sum"
	//args 为   " myrpc 请求i "
	call := <-client.Go(serviceMethod, args, reply, make(chan *Call, 1)).Done
	return call.Error
}
