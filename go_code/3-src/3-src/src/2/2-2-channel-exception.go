package main

import "fmt"

func send(ch chan int) {
	i := 0
	for {
		i++
		ch <- i
	}
}

func recv(ch chan int) {
	value := <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	close(ch)
}

// 向一个已经关闭的通道执行写操作会抛出异常，这意味着我们在写通道时一定要确保通道没有被关闭。
func main() {
	var ch = make(chan int, 4)
	go recv(ch)
	send(ch)
}
