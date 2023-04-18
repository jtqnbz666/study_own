/*
2-7 多路通道
在真实的世界中，还有一种消息传递场景，那就是消费者有多个消费来源，只要有一个来源生产了数据，
消费者就可以读这个数据进行消费。这时候可以将多个来源通道的数据汇聚到目标通道，然后统一在目标通道进行消费。

*/
package main

import (
	"fmt"
	"time"
)

// 每隔一会生产一个数
func send(ch chan int, gap time.Duration) {
	i := 0
	for {
		i++
		ch <- i
		time.Sleep(gap)
	}
}

// 将多个原通道内容拷贝到单一的目标通道
func collect(source chan int, target chan int) {
	for v := range source {
		target <- v // ch3 <- ch2 ; ch3 <- ch1
	}
}

func collect2(ch1 chan int, ch2 chan int, target chan int) {
	for {
		select {
		case v := <-ch1:
			target <- v
		case v := <-ch2:
			target <- v
		default: // 非阻塞
			fmt.Println("collect2")
		}
	}
}

// 从目标通道消费数据
func recv(ch chan int) {
	for v := range ch {
		fmt.Printf("receive %d\n", v)
	}
}

func main() {
	var ch1 = make(chan int)
	var ch2 = make(chan int)
	var ch3 = make(chan int)
	go send(ch1, time.Second)
	go send(ch2, 2*time.Second)
	// go collect(ch1, ch3)
	// go collect(ch2, ch3)
	go collect2(ch1, ch2, ch3)
	recv(ch3)
}
