//简单结构体操作
package main

import (
	"fmt"
	"reflect"
)

type Skills interface {
	reading()
	running()
}

type Student struct {
	Name string
	Age  int
}

func (self Student) runing() {
	fmt.Printf("%s is running\n", self.Name)
}
func (self Student) reading() {
	fmt.Printf("%s is reading\n", self.Name)
}
func main() {
	stu1 := Student{Name: "darren", Age: 18}
	stu_val := reflect.ValueOf(stu1)                //获取Value类型
	fmt.Println(stu_val.NumField())                 //2
	fmt.Println(stu_val.Field(0), stu_val.Field(1)) //darren 18
	fmt.Println(stu_val.FieldByName("Age"))         //18

	stu_val2 := reflect.ValueOf(&stu1).Elem() // 要修改传引用或者指针
	stu_val2.FieldByName("Age").SetInt(33)    //设置字段值 ，结果33
	fmt.Println(stu1.Age)

}
