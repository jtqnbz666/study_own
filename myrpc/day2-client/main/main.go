package main

import (
	"fmt"
	"log"
	"myrpc"
	"net"
	"sync"
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
	//异步
	log.SetFlags(0)
	addr := make(chan string)
	go startServer(addr)

	client, _ := myrpc.Dial("tcp", <-addr)
	defer func() { _ = client.Close() }()

	time.Sleep(time.Second)

	//发送和接收
	var wg sync.WaitGroup
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			args := fmt.Sprintf("myrpc 请求%d", i)
			var reply string
			if err := client.Call("Foo.Sum", args, &reply); err != nil {
				log.Fatal("call Foo.Sum 出错:", err)
			}
			log.Println("reply:", reply)
		}(i)
	}
	wg.Wait()
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
