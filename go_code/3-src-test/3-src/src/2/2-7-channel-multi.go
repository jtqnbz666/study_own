//多路通道
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

func collect(source chan int, target chan int) {
	for v := range source {
		target <- v
	}
}

func collect2(ch1 chan int, ch2 chan int, target chan int) {
	for {
		select {
		case v := <-ch1:
			target <- v
		case v := <-ch2:
			target <- v
		default: //非阻塞	 若不要default ，会阻塞在上边管道出，可以把for去掉测试证明
			//fmt.Println("collect2")
		}
	}
}

func recv(ch chan int) {
	for v := range ch {
		fmt.Println("receive %d\n", v)
	}
}
func main() {
	var ch1 = make(chan int)
	var ch2 = make(chan int)
	var ch3 = make(chan int)
	go send(ch1, time.Second)
	go send(ch2, 2*time.Second)

	go collect2(ch1, ch2, ch3)
	recv(ch3)
}
