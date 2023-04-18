// 反射创建引用类型的实例
/*
使用反射来生成通常需要make函数的实例。可以使用reflect.MakeSlice，
reflect.MakeMap和reflect.MakeChan函数制作切片，Map或通道。
在所有情况下，都提供一个reflect.Type，然后获取一个reflect.Value，
可以使用反射对其进行操作，或者可以将其分配回一个标准变量。
*/
package main

import (
	"fmt"
	"reflect"
)

func main() {
	// 定义变量
	intSlice := make([]int, 0)
	mapStringInt := make(map[string]int)

	// 获取变量的 reflect.Type
	sliceType := reflect.TypeOf(intSlice)
	mapType := reflect.TypeOf(mapStringInt)

	// 使用反射创建类型的新实例
	intSliceReflect := reflect.MakeSlice(sliceType, 0, 0)
	mapReflect := reflect.MakeMap(mapType)

	// 将创建的新实例分配回一个标准变量
	v := 10
	rv := reflect.ValueOf(v)
	intSliceReflect = reflect.Append(intSliceReflect, rv)
	intSlice2 := intSliceReflect.Interface().([]int)
	fmt.Println("intSlice2: ", intSlice2)
	fmt.Println("intSlice : ", intSlice)

	k := "hello"
	rk := reflect.ValueOf(k)
	mapReflect.SetMapIndex(rk, rv)
	mapStringInt2 := mapReflect.Interface().(map[string]int)
	fmt.Println("mapStringInt2: ", mapStringInt2)
	fmt.Println("mapStringInt : ", mapStringInt)
}
