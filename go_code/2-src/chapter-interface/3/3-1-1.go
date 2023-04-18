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
	Age  int
	Name string
}

func (self Student) runing() {
	fmt.Printf("%s is running\n", self.Name)
}
func (self Student) reading() {
	fmt.Printf("%s is reading\n", self.Name)
}
func main() {
	stu1 := Student{Name: "darren", Age: 34}
	inf := new(Skills)
	stu_type := reflect.TypeOf(stu1)
	inf_type := reflect.TypeOf(inf).Elem() //  获取指针所指的对象类型
	fmt.Println("类型stu_type:", stu_type)
	fmt.Println(stu_type.String())  //main.Student
	fmt.Println(stu_type.Name())    //Student
	fmt.Println(stu_type.PkgPath()) //main
	fmt.Println(stu_type.Kind())    //struct
	fmt.Println(stu_type.Size())    //24

	fmt.Println("\n类型inf_type:", inf_type)
	fmt.Println(inf_type.NumMethod())                        //2
	fmt.Println(inf_type.Method(0), inf_type.Method(0).Name) // {reading main func() <invalid Value> 0} reading
	fmt.Println(inf_type.MethodByName("reading"))            //{reading main func() <invalid Value> 0} true

}
