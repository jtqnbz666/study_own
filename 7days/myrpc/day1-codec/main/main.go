package main

import (
	"encoding/json"
	"fmt"
	"log"
	"myrpc"
	"myrpc/codec"
	"net"
	"time"
)

func startServer(addr chan string) {
	//挑选一个空闲的端口
	l, err := net.Listen("tcp", ":0")
	if err != nil {
		log.Fatal("network error:", err)
	}
	log.Println("start rpc server on", l.Addr())
	addr <- l.Addr().String()
	myrpc.Accept(l)
}

func main() {
	log.SetFlags(0)
	addr := make(chan string)
	go startServer(addr)

	conn, _ := net.Dial("tcp", <-addr)
	defer func() { _ = conn.Close() }()

	time.Sleep(time.Second)
	_ = json.NewEncoder(conn).Encode(myrpc.DefaultOption) // 编码的同时已经发送出去了
	cc := codec.NewGobCodec(conn)                         // 生成了一个编码器cc， gob类型的
	//发送和接收
	for i := 0; i < 5; i++ {
		h := &codec.Header{
			ServiceMethod: "Foo.Sum", //请求方法
			Seq:           uint64(i), //请求序列号
		}
		_ = cc.Write(h, fmt.Sprintf("myrpc 请求 %d", h.Seq))
		_ = cc.ReadHeader(h) // 先把头读走.
		var reply string
		_ = cc.ReadBody(&reply)
		log.Println("reply:", reply)
	}
}
