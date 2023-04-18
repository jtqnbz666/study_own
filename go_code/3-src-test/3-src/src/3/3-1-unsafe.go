package main

import "fmt"

type Person struct {
	Name string
	Age  int
}
type Student struct {
	score   string
	Age     int
	*Person //匿名对象
}

//如果要初始化这个匿名对象， 可以理解为Student结构体中有一个匿名对象的名字和结构体名字一样

//如果匿名对象中有和当前类相同的成员，如果是匿名的方式优先调用自己的同名成员
func main() {
	stu := &Student{
		//如果是*Person 这里就换成Person: &Person{}
		Person: &Person{
			Name: "jt",
			Age:  18,
		}, // 这里必须加逗号
		Age: 12,
	}
	fmt.Println(stu.Name)
}
