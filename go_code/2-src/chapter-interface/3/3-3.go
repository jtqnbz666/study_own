package main

import (
	"fmt"
	"reflect"
)

func test1() {
	var s int = 42
	var v = reflect.ValueOf(s)
	v.SetInt(43)
	fmt.Println(s)
}
func test2() {
	var s int = 42
	// 反射指针类型
	var v = reflect.ValueOf(&s)
	// 要拿出指针指向的元素进行修改
	v.Elem().SetInt(43)
	fmt.Println(s)
}
func main() {
	test1()
}
