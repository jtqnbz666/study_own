package main

import (
	"context"
	"log"
	"myrpc"
	"net"
	"net/http"
	"sync"
	"time"
)

type Foo int
type Args struct{ Num1, Num2 int }

func (f Foo) Sum(args Args, reply *int) error {
	*reply = args.Num1 + args.Num2
	return nil
}

func startServer(addr chan string) {
	var foo Foo
	if err := myrpc.Register(&foo); err != nil {
		log.Fatal("注册服务失败", err)
	}
	//挑选一个空闲的端口
	l, err := net.Listen("tcp", ":9999")
	if err != nil {
		log.Fatal("network error:", err)
	}
	log.Println("start rpc server on", l.Addr())
	myrpc.HandleHTTP()
	addr <- l.Addr().String()
	_ = http.Serve(l, nil)
	// myrpc.Accept(l)
}

func call(addr chan string) {
	client, _ := myrpc.DialHTTP("tcp", <-addr)
	defer func() { _ = client.Close() }()

	time.Sleep(time.Second)

	//发送和接收
	var wg sync.WaitGroup
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			args := &Args{Num1: i, Num2: i * i}
			//args := fmt.Sprintf("myrpc 请求%d", i)
			var reply int
			if err := client.Call(context.Background(), "Foo.Sum", args, &reply); err != nil {
				log.Fatal("call Foo.Sum 出错:", err)
			}
			log.Printf("%d + %d = %d", args.Num1, args.Num2, reply)
		}(i)
	}
	wg.Wait()
}

func main() {
	//异步
	log.SetFlags(0)
	addr := make(chan string)
	go call(addr)
	startServer(addr)

}

/* 异步
start rpc server on [::]:34885
&{Foo.Sum 5 } myrpc 请求3
&{Foo.Sum 2 } myrpc 请求4
&{Foo.Sum 1 } myrpc 请求0
&{Foo.Sum 3 } myrpc 请求2
&{Foo.Sum 4 } myrpc 请求1
reply: myrpc resp 3
reply: myrpc resp 4
reply: myrpc resp 2
reply: myrpc resp 1
reply: myrpc resp 5
*/
