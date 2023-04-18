package main

import "fmt"

type IntVetor []int

func (iv IntVetor) Sum(param int) (re int) {
	su := 0
	for _, v := range iv {
		su = su + v
	}
	re = su + param
	return
}

// 非结构体上的例子
func main() {
	tv := IntVetor{3, 4, 5}
	fmt.Println(tv.Sum(333))
}
