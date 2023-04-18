// 2.2 读写通道

package main

import (
	"fmt"
	"time"
)

func main() {
	ch := make(chan float32, 4)
	for i := 0; i < cap(ch); i++ {
		ch <- 1.0 // 写通道
	}
	for len(ch) > 0 {
		value := <-ch // 读通道
		fmt.Println(value)
	}

	// ch1 := make(chan int, 1) // 这里是缓存 有一个1元素
	ch1 := make(chan int) // 非缓存的，实际是0个，并不是1个

	go func() {
		time.Sleep(1 * time.Second)
		ch1 <- 1
		ch1 <- 1             // 这里已经阻塞
		fmt.Println("写入ch1") //这里没打印
	}()

	value1 := <-ch1
	value1 = <-ch1
	time.Sleep(5 * time.Second)
	fmt.Println("退出, value1:", value1)
}
