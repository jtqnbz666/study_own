// 2-8 多路复用select
package main

import (
	"fmt"
	"time"
)

func send(ch chan int, gap time.Duration) {
	i := 0
	for {
		i++
		ch <- i
		time.Sleep(gap)
	}
}

func recv(ch1 chan int, ch2 chan int) {
	for {
		select {
		case v := <-ch1:
			fmt.Printf("recv %d from ch1\n", v)
		case v := <-ch2:
			fmt.Printf("recv %d from ch2\n", v)
		}
	}
}

func main() {
	var ch1 = make(chan int)
	var ch2 = make(chan int)
	go send(ch1, time.Second)
	go send(ch2, 2*time.Second)
	recv(ch1, ch2)
}
