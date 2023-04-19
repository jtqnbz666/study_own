package main

import (
	"context"
	"fmt"
	"time"
)

type MyInt struct {
	done chan struct{}
}

func (t *MyInt) Done() <-chan struct{} { // 没有用
	if t.done == nil {
		t.done = make(chan struct{})
	}
	tmp := t.done
	return tmp
}

func main() {
	ctx, cancel1 := context.WithCancel(context.Background())
	//var jt MyInt
	jt := new(MyInt) //这两种初始化方式都可以
	//jt.done = make(chan struct{})
	go func() {
		time.Sleep(time.Second * 1)
		close(jt.done)
	}()

	go func(ctx context.Context) {
		for {
			select {
			case hh := <-jt.Done():
				fmt.Println("jttest", hh)
				fmt.Println("jt这里监控结束了，1s时间")
				return
			case v := <-ctx.Done():
				fmt.Println("监控退出， 停止了。 。。, v :", v, ", err :", ctx.Err())
				return
			default:
				time.Sleep(2 * time.Second)
				fmt.Println("goroutine 监控中")
			}
		}
	}(ctx)

	time.Sleep(5 * time.Second)
	fmt.Println("可以了， 通知监控停止")
	cancel1()

	time.Sleep(5 * time.Second)
}
