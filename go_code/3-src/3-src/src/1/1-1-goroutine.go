// 1.1 如何使用Goroutines

package main

import (
	"fmt"
	"time"
)

func hello(name string) {
	for {
		fmt.Println("Hello world :", name)
		time.Sleep(1 * time.Second)
	}
}
func main() {
	go hello("tt") // go关键字 把函数放到协程去运行
	time.Sleep(5 * time.Second)
	fmt.Println("main function")
}
