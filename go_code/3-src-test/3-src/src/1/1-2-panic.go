package main

import (
	"fmt"
	"time"
)

func main() {
	fmt.Println("run in main goroutine ")
	go func() {
		go func() {
			defer fmt.Println("异常") //尽管崩溃， 还是执行了
			fmt.Println("run in grand child goroutine")
			fmt.Println("run in child goroutine")
			var ptr *int
			//ptr = new(int)
			*ptr = 0x12345 //制造崩溃
			go func() {
				fmt.Println("run in grand grand child goroutine")
			}()
		}()
		time.Sleep(time.Second * 1)
		fmt.Println("离开， run in child goroutine leave")
	}()
	time.Sleep(time.Second * 1)
	fmt.Println("main goroutine will quit")
}
