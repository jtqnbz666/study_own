//通过反射调用结构体中的方法，通过reflect.Value.Method(i int).Call()
//或者reflect.Value.MethodByName(name string).Call()实现
package main

import (
	"fmt"
	"reflect"
)

type Student struct {
	Name string
	Age  int
}

func (this *Student) SetName(name string) {
	this.Name = name
	fmt.Printf("set name %s\n", this.Name)
}

func (this *Student) SetAge(age int) {
	this.Age = age
	fmt.Printf("set age %d\n", age)
}

func (this *Student) String() string {
	fmt.Printf("this is %s\n", this.Name)
	return this.Name
}

func (this *Student) SetAgeAndName(age int, name string) {
	this.Age = age
	fmt.Printf("set age %d, name:%s\n", age, name)
}

func main() {
	stu1 := &Student{Name: "darren", Age: 18}
	val := reflect.ValueOf(stu1)         //获取Value类型，也可以使用reflect.ValueOf(&stu1).Elem()
	val.MethodByName("String").Call(nil) //调用String方法

	params := make([]reflect.Value, 1)
	params[0] = reflect.ValueOf(18)
	val.MethodByName("SetAge").Call(params) //通过名称调用方法

	params[0] = reflect.ValueOf("king")
	// val.Method(1).Call(params) //通过方法索引调用
	val.Method(2).Call(params) //通过方法索引调用	通过索引的方式拿到函数不安全
	fmt.Println(stu1.Name, stu1.Age)

	params = make([]reflect.Value, 2)
	params[0] = reflect.ValueOf(18)
	params[1] = reflect.ValueOf("柚子老师")
	val.MethodByName("SetAgeAndName").Call(params)
}

//this is darren
//set age 18
//set name king
//king 18
