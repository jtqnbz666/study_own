// 读写管道
package main

import (
	"fmt"
	"time"
)

func receive(ch chan float32) {
	value := <-ch // 读管道
	fmt.Println(value)
}

func main() {
	ch := make(chan float32)

	go receive(ch)
	ch <- 1.0 //写管道
	//for i := 0; i < 1; i++ {

	//}
	//for len(ch) > 0 {
	err := recover()
	fmt.Println(err)

	//}

	ch1 := make(chan int, 1) //这里是缓存， 有一个1元素
	//ch1 := make(chan int)

	go func() {
		time.Sleep(1 * time.Second)
		ch1 <- 1
		ch1 <- 1             //这里已经阻塞
		fmt.Println("写入ch1") //这里没有打印
	}()

	value1 := <-ch1
	//value1 = <-ch1 //若加上这一句， 就可以不用阻塞
	time.Sleep(5 * time.Second)
	fmt.Println("退出， value1:", value1)
}
