//waitGroup 在写端关闭channel对单写的程序有效，但是多写的时候就有问题
package main

import(
	"fmt"
	"sync"
	"time"
)

func send(ch chan int, wg *sync.WaitGroup) {
	defer wg.Done() //计数减一
	i := 0
	for i < 4 {
		i ++
		ch <- i
	}
}

func recv(ch chan int) {
	for v := range ch {
		fmt.Println(v)
	}
}

//只要一个值能做定界符， 比如nil，

func main () {
	var ch = make(chan int, 4) 
	var wg = new(sync.WaitGroup)
	wg.Add(2)
	go send(ch, wg)
	go send(ch, wg)
	go recv(ch)

	//wait()阻塞等待所有的写通道协程结束
	//待计数值变成 0 ， wait返回
	wg.Wait() 
	//关闭通道
	close(ch)
	time.Sleep(time.Second)
}