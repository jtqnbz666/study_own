package main

import (
	"fmt"
	"strings"
)

// 缩小变量作用域，减少对全局变量的污染。下面的累加如果用全局变量进行实现，全局变量容易被其他人污染。
// 同时，所有我要实现n个累加器，那么每次需要n个全局变量。利用闭包，
// 每个生成的累加器myAdder1, myAdder2 := adder(), adder()有自己独立的sum，sum可以看作为myAdder1.sum与myAdder2.sum。
func Adder() func(int) int {
	var x int
	f := func(d int) int {
		x += d
		return x
	}
	return f
}

func makeSuffix(suffix string) func(string) string {
	f := func(name string) string {

		if strings.HasSuffix(name, suffix) == false {
			return name + suffix
		}
		return name
	}

	return f
}

func main() {

	f := Adder()
	fmt.Println(f(1))
	fmt.Println(f(100))
	// fmt.Println(f(1000))
	/*
		f1 := makeSuffix(".bmp")
		fmt.Println(f1("test"))
		fmt.Println(f1("pic"))

		f2 := makeSuffix(".jpg")
		fmt.Println(f2("test"))
		fmt.Println(f2("pic"))
	*/

}
