package main

import (
	"encoding/json"
	"fmt"
)

// stu:  序列化后：{"name":"darren","age":34}
// 				 {"name1":"darren","age2":34}
type Student struct {
	Name string `json:"name1"`
	Age  int    `json:"age2"`
}

func main() {
	var stu = Student{Name: "darren", Age: 34}
	data, err := json.Marshal(stu) //   {"name1":"darren","age2":34}
	if err != nil {
		fmt.Println("json encode failed err:", err)
		return
	}
	fmt.Println("stu: ", string(data)) //{"name":"darren","age":34}

	var stu2 Student
	err = json.Unmarshal(data, &stu2) // 反序列化 
	fmt.Println("stu2: ", stu2)       // {darren 34}
}
