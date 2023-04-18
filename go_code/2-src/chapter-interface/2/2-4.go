// 2.4 interface多态

package main

import "fmt"

type Skills interface {
	Running()
	Getname() string
}

type Student struct {
	Name string
	Age  int
}

type Teacher struct {
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
}

//darren running
//king running
