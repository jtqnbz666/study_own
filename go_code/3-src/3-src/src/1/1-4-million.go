// 1-4 启动百万协程

package main

import (
	"fmt"
	"runtime"
	"time"
)

const N = 1000000

func main() {
	fmt.Println("run in main goroutine")
	i := 1
	for {
		go func() {
			for {
				time.Sleep(time.Second)
			}
		}()
		if i%10000 == 0 {
			fmt.Printf("%d goroutine started\n", i)
		}
		i++
		if i == N {
			break
		}
	}
	fmt.Println("NumGoroutine:", runtime.NumGoroutine())
	time.Sleep(time.Second * 15)
}
