package main

import (
	"fmt"
)

func main() {
	var ch = make(chan int, 4)
	ch <- 1
	ch <- 2
	fmt.Println("len(ch):", len(ch), "cap(ch):", cap(ch))
	close(ch)

	value := <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	ch <- 3 //产生错误
}
