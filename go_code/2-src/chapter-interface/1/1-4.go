// 1.4 struct匿名成员（字段、属性)

package main

import "fmt"

type Person struct {
	Name string
	Age  int
}
type Student struct {
	score  string
	Age    int
	Person // 匿名内嵌结构体
}

type son struct {
	a int
}
type father struct {
	*son
	b int
}

func main() {
	var stu = new(Student)
	stu.Age = 22                         //优先选择Student中的Age
	fmt.Println(stu.Person.Age, stu.Age) // 0,22

	var stu2 = Student{
		score: "100",
		Age:   20,
		Person: Person{
			Name: "柚子",
			Age:  18,
		},
	}
	stu5 := &Student{
		Person: Person{
			Name: "jt",
			Age:  18,
		},
	}
	stu3 := Student{score: "100", Age: 20}
	fmt.Println("stu2: ", stu2)
	fmt.Println("stu3: ", stu3)
	fmt.Println("stu5: ", stu5)

	tt := &father{
		b:   10,
		son: &son{a: 6},
	}
	fmt.Println(tt.a)

}
