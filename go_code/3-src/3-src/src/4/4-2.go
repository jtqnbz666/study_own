package main

import (
	"context"
	"fmt"
	"sync"
	"time"
)

func work(ctx context.Context, wg *sync.WaitGroup) {
	defer wg.Done()
	for {
		select {
		case <-ctx.Done():
			fmt.Println("监控退出，停止了...")
			return
		default:
			fmt.Println("hello")
			time.Sleep(time.Second)
		}
	}
}

func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel() // 在建立之后，立即 defer cancel() 是一个好习惯。
	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go work(ctx, &wg)
	}
	time.Sleep(time.Second)

	wg.Wait()
}
