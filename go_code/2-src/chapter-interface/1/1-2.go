package main

import "fmt"

type Student struct {
	name  string
	age   int
	Class string
}

func Newstu(name1 string, age1 int, class1 string) *Student {
	return &Student{name: name1, age: age1, Class: class1}
}
func main() {
	stu1 := Newstu("darren", 34, "math")
	fmt.Println(stu1.name) // darren
}
