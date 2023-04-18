package main

import (
	"fmt"
	"reflect"
)

type Rect struct {
	Width  int
	Height int
	Name   string
}

// 通过统一的接口去实现属性设置的
func SetRectAttr(r *Rect, name string, value int) {
	var v = reflect.ValueOf(r)
	var field = v.Elem().FieldByName(name)
	field.SetInt(int64(value))
}

// 结构体也是值类型，也必须通过指针类型来修改。
func main() {
	var r = Rect{50, 100}
	SetRectAttr(&r, "Width", 100) // 修改属性的接口
	SetRectAttr(&r, "Height", 200)
	SetRectAttr(&r, "Name", "正方形")
	fmt.Println(r)
}
