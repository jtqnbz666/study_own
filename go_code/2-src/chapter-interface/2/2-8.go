package main

import "fmt"

type Student struct {
	Name string
}

func TestType(items ...interface{}) {
	for k, v := range items {
		switch v.(type) {
		case string:
			fmt.Printf("type is string, %d[%v]\n", k, v)
		case bool:
			fmt.Printf("type is bool, %d[%v]\n", k, v)
		case int:
			fmt.Printf("type is int, %d[%v]\n", k, v)
		case float32, float64:
			fmt.Printf("type is float, %d[%v]\n", k, v)
		case Student:
			fmt.Printf("type is Student, %d[%v]\n", k, v)
		case *Student:
			fmt.Printf("type is Student, %d[%p]\n", k, v)
		}
	}
}

func main() {
	var stu Student
	TestType("darren", 100, stu, 3.3)
}

//type is string, 0[darren]
//type is int, 1[100]
//type is Student, 2[{}]
//type is float, 3[3.3]
