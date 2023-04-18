// 2-5 通道写安全
package main

import "fmt"

func send(ch chan int) { // 在写入端关闭, 没有太多的通用性
	ch <- 1
	ch <- 2
	ch <- 3
	ch <- 4
	close(ch)
}

func recv(ch chan int) {
	for v := range ch {
		fmt.Println(v)
	}
	value := <-ch // 判别不了是否已经读取完毕
	fmt.Println("value:", value)
}

// 确保通道写安全的最好方式是由负责写通道的协程自己来关闭通道，读通道的协程不要去关闭通道。
func main() {
	var ch = make(chan int, 1)
	go send(ch)
	recv(ch)
}
