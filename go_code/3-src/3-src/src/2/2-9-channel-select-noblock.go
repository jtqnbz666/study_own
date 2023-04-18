// 2-9 非阻塞读写
package main

import (
	"fmt"
	"time"
)

func send(ch1 chan int, ch2 chan int) {
	i := 0
	for {
		i++
		select {
		case ch1 <- i:
			fmt.Printf("send ch1 %d\n", i)
		case ch2 <- i:
			fmt.Printf("send ch2 %d\n", i)
		default:
			fmt.Printf("ch block\n")
			time.Sleep(2 * time.Second) // 这里只是为了演示
		}
	}
}

func recv(ch chan int, gap time.Duration, name string) {
	for v := range ch {
		fmt.Printf("receive %s %d\n", name, v)
		time.Sleep(gap)
	}
}

func main() {
	// 无缓冲通道
	var ch1 = make(chan int)
	var ch2 = make(chan int)
	// 两个消费者的休眠时间不一样，名称不一样
	go recv(ch1, time.Second, "ch1")
	go recv(ch2, 2*time.Second, "ch2")
	send(ch1, ch2)
}
