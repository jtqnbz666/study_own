//	协程异常处理 -- recover
package main

import(
	"fmt"
	"time"
)

func main() {
	fmt.Println("run in main goroutine")
	go func() {
		fmt.Println("run in child goroutine")
		defer func() {	//要在对应的协程里执行
			fmt.Println("执行defer:")
			if err := recover(); err != nil {
				fmt.Println("捕获error:", err)
			}
		}()
		fmt.Println("run in grand grand child goroutine")
		var ptr *int
		*ptr = 0x12345
		go func() {
			fmt.Println("吱吱run in grand child goroutine")
			go func() {

			}()
		}()
		//time.Sleep(time.Second * 1)
		fmt.Println("离开： run in child goroutine leave")
	}()
	time.Sleep(2 * time.Second)
	fmt.Println("main goroutine will quit")
}