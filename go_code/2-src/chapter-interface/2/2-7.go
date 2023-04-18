// 2.7 interface类型转换

package main

import "fmt"

func main() {
	var x interface{}

	s := "darren"
	x = s // 为什么能赋值到空接口, 每种类型都已经隐藏实现了空接口
	y, ok := x.(int)
	z, ok1 := x.(string)
	fmt.Println(y, ok)
	fmt.Println(z, ok1)
}

//0 false
//darren true
