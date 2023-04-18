// 获取和设置普通类型的值
package main

import (
	"fmt"
	"reflect"
)

func main() {
	str := "darren"
	age := 11
	fmt.Println(reflect.ValueOf(str).String()) //获取str的值，结果darren
	fmt.Println(reflect.ValueOf(age).Int())    //获取age的值，结果age
	str2 := reflect.ValueOf(&str)              //获取Value类型
	str2.Elem().SetString("king")              //设置值
	fmt.Println(str2.Elem(), age)              //king 11

	age2 := reflect.ValueOf(&age) //获取Value类型
	fmt.Println("age2:", age2)

	age2.Elem().SetInt(40) //设置值
	fmt.Println("age:", age)
	fmt.Println("reflect.ValueOf(age):", reflect.ValueOf(age))
}
