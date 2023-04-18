package main

import "fmt"

type TwoInts struct {
	a int
	b int
}

func (ti *TwoInts) Add() int {
	return ti.a + ti.b
}

func (ti *TwoInts) Plus() (re int) {
	re = ti.a * ti.b
	return
}

func (ti *TwoInts) AddOther(param int) (re int) {
	re = ti.a + ti.b + param
	return
}

// 结构体上的例子
func main() {
	tt := new(TwoInts)
	tt.a = 10
	tt.b = 20

	fmt.Println(tt.Add())
	fmt.Println(tt.Plus())

	ttt := TwoInts{3, 5}
	fmt.Println(ttt.AddOther(7))
}
