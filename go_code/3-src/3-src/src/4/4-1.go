package main

import (
	"context"
	"fmt"
	"time"
)

func main() {
	ctx, cancel1 := context.WithCancel(context.Background())
	go func(ctx context.Context) {
		for {
			select {
			case v := <-ctx.Done():
				fmt.Println("监控退出，停止了..., v: ", v, ", err:", ctx.Err())
				return
			default:
				time.Sleep(2 * time.Second)
				fmt.Println("goroutine监控中...")
				// time.Sleep(2 * time.Second)
			}
		}
	}(ctx)

	time.Sleep(5 * time.Second)
	fmt.Println("可以了，通知监控停止")
	cancel1()

	//为了检测监控过是否停止，如果没有监控输出，就表示停止了
	time.Sleep(5 * time.Second)
}
