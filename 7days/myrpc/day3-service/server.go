package myrpc

import (
	"encoding/json"
	"errors"
	"io"
	"log"
	"myrpc/codec"
	"net"
	"reflect"
	"strings"
	"sync"
)

const MagicNumber = 0x3bef5c

type Option struct {
	MagicNumber int
	CodecType   codec.Type
}

var DefaultOption = &Option{
	MagicNumber: MagicNumber,
	CodecType:   codec.GobType, // 默认使用gob的编码方式
}

type Server struct {
	serviceMap sync.Map //去重用的
}

func NewServer() *Server {
	return &Server{}
}

var DefaultServer = NewServer()

func (server *Server) ServeConn(conn io.ReadWriteCloser) {
	defer func() { _ = conn.Close() }()
	var opt Option
	if err := json.NewDecoder(conn).Decode(&opt); err != nil {
		log.Println("rpc. 解析options头部失败:", err)
		return
	}
	if opt.MagicNumber != MagicNumber {
		log.Printf("rpc 解析 magic number 失败 %x", opt.MagicNumber)
		return
	}
	// 根据客户端的编码方式，服务端生成一个相同的方法
	f := codec.NewCodecFuncMap[opt.CodecType] //返回的f是一个函数指针
	if f == nil {
		log.Printf("rpc 生成编码器失败%s", opt.CodecType)
		return
	}
	server.serveCodec(f(conn))
}

var invalidRequest = struct{}{}

func (server *Server) serveCodec(cc codec.Codec) {
	sending := new(sync.Mutex)
	wg := new(sync.WaitGroup)
	for {
		req, err := server.readRequest(cc) //先接收请求，下边再去处理请求
		if err != nil {
			if req == nil {
				break //没有东西, 出错了就退出
			}
			req.h.Error = err.Error() // 把错误记录在请求头部的error字段
			server.sendResponse(cc, req.h, invalidRequest, sending)
			continue
		}
		wg.Add(1)
		go server.handleRequest(cc, req, sending, wg)
	}
	wg.Wait()
	_ = cc.Close()
}

// 一个请求分为 head 和 body， 将它们封装为一个结构体
type request struct {
	h            *codec.Header //请求头， 像调用的方法啊啥的
	argv, replyv reflect.Value //请求或者回应的字段
	mType        *methodType
	svc          *service
}

func (server *Server) readRequestHeader(cc codec.Codec) (*codec.Header, error) {
	var h codec.Header
	if err := cc.ReadHeader(&h); err != nil {
		if err != io.EOF && err != io.ErrUnexpectedEOF {
			log.Println("rpc 读取头部的时候失败:", err)
		}
		return nil, err
	}
	return &h, nil
}

func (server *Server) readRequest(cc codec.Codec) (*request, error) {
	h, err := server.readRequestHeader(cc)
	if err != nil {
		return nil, err
	}
	req := &request{h: h}
	req.svc, req.mType, err = server.findService(h.ServiceMethod)
	if err != nil {
		return req, err
	}
	req.argv = req.mType.newArgv()
	req.replyv = req.mType.newReplyv()

	//确保argvi 是一个指针， readbody中解码需要一个指针作为参数
	argvi := req.argv.Interface()
	if req.argv.Type().Kind() != reflect.Ptr {
		argvi = req.argv.Addr().Interface() //如果不是指针就转化为指针
	}
	if err = cc.ReadBody(argvi); err != nil {
		log.Println("rpc server: 读body的时候失败:", err)
		return req, err
	}
	return req, nil
	// 并不知道请求的方法类型
	req.argv = reflect.New(reflect.TypeOf(""))
	if err = cc.ReadBody(req.argv.Interface()); err != nil {
		log.Println("rpc server: 读取body的时候失败：", err)
	}
	return req, nil
}

func (server *Server) sendResponse(cc codec.Codec, h *codec.Header, body interface{}, sending *sync.Mutex) {
	sending.Lock()
	defer sending.Unlock()
	if err := cc.Write(h, body); err != nil {
		log.Println("rpc写回应的时候失败", err)
	}
}

func (server *Server) handleRequest(cc codec.Codec, req *request, sending *sync.Mutex, wg *sync.WaitGroup) {
	defer wg.Done()
	err := req.svc.call(req.mType, req.argv, req.replyv)
	if err != nil {
		req.h.Error = err.Error()
		server.sendResponse(cc, req.h, invalidRequest, sending)
		return
	}
	server.sendResponse(cc, req.h, req.replyv.Interface(), sending)
}

func (server *Server) Accept(lis net.Listener) {
	for {
		conn, err := lis.Accept()
		if err != nil {
			log.Println("rpc , accept失败:", err)
			return
		}
		go server.ServeConn(conn)
	}
}

func Accept(lis net.Listener) { DefaultServer.Accept(lis) }

//注册服务
func (server *Server) Register(recv interface{}) error {
	s := newService(recv)
	if _, dup := server.serviceMap.LoadOrStore(s.name, s); dup {
		return errors.New("rpc: 服务已经定义过了-" + s.name)
	}
	return nil
}

func Register(recv interface{}) error { //提供给外界的接口
	return DefaultServer.Register(recv)
}

//发现服务
func (server *Server) findService(serviceMethod string) (svc *service, mtype *methodType, err error) {
	dot := strings.LastIndex(serviceMethod, ".") //Foo.Sum  相当于拆解它
	if dot < 0 {
		err = errors.New("rpc server: 请求的rpc方法错误")
		return
	}
	serviceName, methodName := serviceMethod[:dot], serviceMethod[dot+1:]
	svci, ok := server.serviceMap.Load(serviceName) //去map表中查找是否注册了这个服务
	if !ok {
		err = errors.New("rcp server: 没有找到这个服务")
		return
	}
	svc = svci.(*service) //接口类型的转换
	mtype = svc.method[methodName]
	if mtype == nil {
		err = errors.New("rpc server: 没有发现这个方法")
	}
	return
}
