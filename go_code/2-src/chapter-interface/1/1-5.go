// 1.5 struct-继承、多继承
package main

import "fmt"

type Person struct {
	Name string
	Age  int
}
type Teacher struct {
	Salary int
	Class  string
}

type Man struct {
	sex    string
	job    Teacher //别名，继承Teacher
	Person         //继承Person
}

func main() {
	var man1 = new(Man)
	man1.Age = 34
	man1.Name = "darren"
	man1.job.Salary = 100000
	fmt.Println("man1:", man1, man1.job.Salary) //&{ {8500 } {darren 34}} 8500

	var man2 = Man{
		sex: "女",
		job: Teacher{
			Salary: 8000,
			Class:  "班主任",
		},
		Person: Person{ // 匿名初始化方式
			Name: "柚子老师",
			Age:  18,
		},
	}
	fmt.Println("man2", man2)
}
