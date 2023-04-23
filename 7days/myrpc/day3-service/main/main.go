package main

import (
	"log"
	"myrpc"
	"net"
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
			args := &Args{Num1: i, Num2: i * i}
			//args := fmt.Sprintf("myrpc 请求%d", i)
			var reply int
			if err := client.Call("Foo.Sum", args, &reply); err != nil {
				log.Fatal("call Foo.Sum 出错:", err)
			}
			log.Printf("%d + %d = %d:", args.Num1, args.Num2, reply)
		}(i)
	}
	wg.Wait()
}

/* 异步
rpc server: 注册了Foo . Sum
start rpc server on [::]:38007
4 + 16 = 20:
3 + 9 = 12:
0 + 0 = 0:
1 + 1 = 2:
2 + 4 = 6:
*/
