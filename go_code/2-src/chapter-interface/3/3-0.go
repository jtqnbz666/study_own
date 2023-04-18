// 3 reflect反射 基础
package main

import (
	"fmt"
	"reflect"
)

func main() {
	var s int = 42
	fmt.Println(reflect.TypeOf(s))
	fmt.Println(reflect.ValueOf(s))                   //42
	fmt.Println(reflect.ValueOf(s).Interface().(int)) //42

	// fmt.Println(reflect.TypeOf(reflect.ValueOf(s)))
}
