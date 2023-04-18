package main

import (
	"fmt"
	"time"
)

func send(ch chan int) {
	time.Sleep(time.Second * 3)
	ch <- 1
	ch <- 2
	//close(ch)
	return
	// ch <- 3
	// ch <- 4
	// time.Sleep(time.Second * 10) //验证读取未关闭空管道会阻塞
	//close(ch)
}

func recv(ch chan int) {
	for v := range ch {
		fmt.Println(len(ch))
		fmt.Println(v)
	}
	fmt.Println("test_recv")
	// var value int
	// for i := 0; i < 5; i++ { //如果上边不关闭管道，这里就会死锁
	// 	value = <-ch
	// 	fmt.Println(value)
	// }
	// //value := <-ch
	//fmt.Println("value:", value)
}

//确保管道写安全的最好方式是由负责写通道的协程自己来关闭
func main() {
	var ch = make(chan int, 1)
	go send(ch)
	recv(ch)
}
