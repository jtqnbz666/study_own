package main

import (
	"fmt"
	"time"
)

func main() {
	fmt.Println("run in main goroutine")
	go func() {
		go func() {
			defer fmt.Println("异常了")
			fmt.Println("run in grand child goroutine")
			fmt.Println("run in child goroutine")
			var ptr *int
			*ptr = 0x12345 // 故意制造崩溃
			go func() {
				fmt.Println("run in grand grand child goroutine")

			}()
		}()
		time.Sleep(time.Second * 1)
		fmt.Println("离开: run in child goroutine leave") // 这里能否执行到
	}()
	time.Sleep(time.Second * 1)
	fmt.Println("main goroutine will quit")
}
