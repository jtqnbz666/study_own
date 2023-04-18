package main

import (
	"fmt"
	"sync"
	"time"
)

var once sync.Once

func main() {
	for i, v := range make([]string, 10) {
		once.Do(onces) //只执行第一个once
		fmt.Println("count :", v, "---", i)
	}
	for i := 0; i < 5; i++ {

		go func() {
			once.Do(onced) //如果执行上边， 这里就不执行
			fmt.Println("123")
		}()
	}
	time.Sleep(time.Second * 4)
}

func onces() {
	fmt.Println("执行onces")
}

func onced() {
	fmt.Println("执行onced")
}
