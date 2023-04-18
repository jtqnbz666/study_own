package main

import (
	"context"
	"fmt"
	"time"
)

func work(ctx context.Context, str string) {
	for {
		select {
		case <-ctx.Done():
			fmt.Println("退出 ", str)
			return
		}
	}
}

func main() {
	ctx1 := context.Background()
	ctx2, cancel2 := context.WithCancel(ctx1)
	ctx3, cancel3 := context.WithTimeout(ctx2, time.Second*5)
	ctx4, cancel4 := context.WithTimeout(ctx3, time.Second*3)
	ctx5, cancel5 := context.WithTimeout(ctx4, time.Second*6)
	ctx6 := context.WithValue(ctx5, "userID", 12)
	go work(ctx1, "ctx1")
	go work(ctx2, "ctx2")
	go work(ctx3, "ctx3")
	go work(ctx4, "ctx4")
	go work(ctx5, "ctx5")
	go work(ctx6, "ctx6")

	time.Sleep(1 * time.Second)
	cancel5()
	time.Sleep(5 * time.Second)
	cancel3()
	cancel4()
	cancel5()
	cancel2()
}
