package main

import (
	"fmt"
	"reflect"
)

type Student struct {
	Id   int
	Name string
}

func (s Student) Hello(){
	fmt.Println("我是一个学生")
}

func main() {
	s := Student{Id: 1, Name: "咖啡色的羊驼"}

	// 获取目标对象
	t := reflect.TypeOf(s)
    fmt.Println( "Name is", t.Name(), "and kind is", t.Kind())
    
    //如果把上边s 换成 &s
    //t := reflect.TypeOf(&s)
    //fmt.Println( "Name is", t.Elem().Name(), "and kind is", t.Elem().Kind())
    
	//打印结果
    //Type is Student and kind is struct
    
    // 获取这个结构体类型的大小 t.Size()

	// 获取目标对象的值类型
	v := reflect.ValueOf(s)
    
	// .NumField()来获取其包含的字段的总数
    //注意这里是t， 不是 v 的 NumField()
	for i := 0; i < t.NumField(); i++ {
		// 从0开始获取Student所包含的key
		key := t.Field(i)

		// 通过interface方法来获取key所对应的值
		value := v.Field(i).Interface()

		fmt.Printf("第%d个字段是：%s:%v = %v \n", i+1, key.Name, key.Type, value)
        //第1个字段是：Id:int = 1 
	   //第2个字段是：Name:string = 咖啡色的羊驼
	}

	// 通过.NumMethod()来获取Student里头的方法
	for i:=0;i<t.NumMethod(); i++ {
		m := t.Method(i)
		fmt.Printf("第%d个方法是：%s:%v\n", i+1, m.Name, m.Type)
        //第1个方法是：Hello:func(main.Student)
	}
}
