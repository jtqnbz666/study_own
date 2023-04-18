/*
注意事项：

select语句 只能用于channel信道的IO操作，每个case都必须是一个信道。
如果不设置 default条件，当没有IO操作发生时，select语句就会一直阻塞；
如果有一个或多个IO操作发生时，Go运行时会随机选择一个case执行，但此时将无法保证执行顺序；
对于case语句，如果存在信道值为nil的读写操作，则该分支将被忽略，可以理解为相当于从select语句中删除了这个case；
对于空的 select语句，会引起死锁；
对于在 for中的select语句，不能添加 default，否则会引起cpu占用过高的问题；
*/
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
			fmt.Printf("ch block \n")
			time.Sleep(2 * time.Second) //这里只是为了演示
		}
	}
}

func recv(ch chan int, gap time.Duration, name string) {
	for v := range ch { //这种也算是读管道了
		fmt.Printf("receive %s %d\n", name, v)
		time.Sleep(gap)
	}
}

func main() {
	//无缓冲通道
	var ch1 = make(chan int)
	var ch2 = make(chan int)
	//两个消费者的休眠时间不一样， 名称不一样
	go recv(ch1, time.Second, "ch1")
	go recv(ch2, 2*time.Second, "ch2")
	send(ch1, ch2)
}
