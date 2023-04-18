package main

import (
	"fmt"
	"reflect"
	"time"
)

const N = 1000000

func test1() {
	var sum = 0
	t0 := time.Now()

	for i := 0; i < (N); i++ {
		var s int = 42
		// 反射指针类型
		var v = reflect.ValueOf(&s)
		// 要拿出指针指向的元素进行修改
		v.Elem().SetInt(43)
		sum += s
	}

	elapsed := time.Since(t0)
	fmt.Println("反射赋值 ", "N:", N, "time:", elapsed, ", sum:", sum)
}

func test2() {
	var sum = 0
	t0 := time.Now()

	for i := 0; i < (N); i++ {
		var s int = 42
		s = 43
		sum += s
	}

	elapsed := time.Since(t0)
	fmt.Println("直接赋值 ", "N:", N, "time:", elapsed, ", sum:", sum)
}
func main() {
	test1()
	test2()
}
