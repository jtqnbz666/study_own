// 2.3 interface使用

package main

import "fmt"

//定义接口
type Skills interface {
	Running()
	Getname() string
}

type Student struct {
	Name string
	Age  int
}

type Teacher struct {
	Name string
	Age  int
}

// 实现接口
func (p Student) Getname() string { //实现Getname方法
	fmt.Println(p.Name)
	return p.Name
}

func (p Student) Running() { // 实现 Running方法
	fmt.Printf("%s running\n", p.Name)
}

func (p Teacher) Getname() string { //实现Getname方法
	fmt.Println(p.Name)
	return p.Name
}

func (p Teacher) Running() { // 实现 Running方法
	fmt.Printf("%s running\n", p.Name)
}

func (p Teacher) Running2() { // 实现 Running方法
	fmt.Printf("%s running\n", p.Name)
}

// 想用接口，那就要实现对应接口的所有方法
func main() {
	var skill Skills // 一个接口变量
	var stu1 Student // 结构体变量
	stu1.Name = "darren"
	stu1.Age = 34
	skill = stu1
	skill.Running() //调用接口

	var teacher Teacher = Teacher{"柚子老师", 18}
	skill = teacher
	skill.Running() //调用接口
	teacher.Running()
}
