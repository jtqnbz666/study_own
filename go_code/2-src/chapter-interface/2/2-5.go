// 2.5 interface接口嵌套

package main

import "fmt"

type Skills interface {
	Running()
	// Running(is int)		// 函数名是唯一的
	Getname() string
}

type Test interface {
	Sleeping()
	Skills //继承Skills
}
type Student struct {
	Name string
	Age  int
}

type Teacher struct {
	skill  Skills // skill也只能当变量去用
	Name   string
	Salary int
}

func (p Student) Getname() string { //实现Getname方法
	fmt.Println(p.Name)
	return p.Name
}

func (p Student) Running() { // 实现 Running方法
	fmt.Printf("%s running", p.Name)
}

func (p Teacher) Getname() string { //实现Getname方法
	fmt.Println(p.Name)
	return p.Name
}

func (p Teacher) Running() { // 实现 Running方法
	fmt.Printf("\n%s running", p.Name)
}

func (p Teacher) Sleeping() { // 实现 Running方法
	fmt.Printf("\n%s Sleeping", p.Name)
}
func main() {
	var skill Skills
	var stu1 Student
	var t1 Teacher
	t1.Name = "king"
	stu1.Name = "darren"
	stu1.Age = 22
	skill = stu1
	skill.Running()
	skill = t1
	t1.Running()

	var test Test
	test = t1
	test.Sleeping()

	// test = stu1
}

//darren running
//king running
