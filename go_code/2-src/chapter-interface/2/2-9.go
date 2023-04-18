package main

import "fmt"

type Rect struct {
	Width  int
	Height int
}

func main() {
	var a interface{}
	var r = Rect{50, 50}
	a = &r // 指向了结构体指针

	var rx = a.(*Rect) // 转换成指针类型
	r.Width = 100
	r.Height = 100
	fmt.Println("r:", r)
	fmt.Println("rx:", rx)
	fmt.Printf("rx:%p, r:%p\n", rx, &r)
}
