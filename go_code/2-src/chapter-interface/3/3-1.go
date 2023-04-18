// 3.1 reflect反射-Type

package main

import (
	"fmt"
	"reflect"
)

// 通用方法

func main() {
	str := "darren"
	res_type := reflect.TypeOf(str)
	fmt.Println(res_type) //string

	int1 := 1
	res_type2 := reflect.TypeOf(int1)
	fmt.Println(res_type2) //int
}
