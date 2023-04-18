package main

import (
	"fmt"
	"reflect"
)

// type Student struct {
// 	Name  string // 16
// 	Age   int64  // 8
// 	wight int64  // 8
// 	high  int64  // 8
// 	score int64  // 8
// }
type Student struct {
	Name  string // 16  有两个变量： 指针, length
	Age   int8   //
	wight int64  // 8
	high  int8   // 8
	score int64  // 8
}

func main() {
	var stu1 = new(Student)
	stu1.Name = "一切只为你"
	fmt.Printf("地址分布:")
	fmt.Printf("%p\n", &stu1.Name)
	fmt.Printf("%p\n", &stu1.Age)
	fmt.Printf("%p\n", &stu1.wight)
	fmt.Printf("%p\n", &stu1.high)
	fmt.Printf("%p\n", &stu1.score)
	typ := reflect.TypeOf(Student{})
	fmt.Printf("Struct is %d bytes long\n", typ.Size())
	// We can run through the fields in the structure in order
	n := typ.NumField()
	for i := 0; i < n; i++ {
		field := typ.Field(i) // 反射出filed
		fmt.Printf("%s at offset %v, size=%d, align=%d\n",
			field.Name, field.Offset, field.Type.Size(),
			field.Type.Align())
	}

}
