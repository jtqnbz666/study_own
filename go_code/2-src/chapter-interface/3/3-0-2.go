// 使用反射创建新实例
/*
除了检查变量的类型外，还可以使用反射来读取，设置或创建值。
首先，需要使用refVal := reflect.ValueOf(var)为变量创建一个reflect.Value实例。
如果希望能够使用反射来修改值，则必须使用refPtrVal := reflect.ValueOf(＆var);
获得指向变量的指针。如果不这样做，则可以使用反射来读取该值，但不能对其进行修改。

一旦有了reflect.Value实例就可以使用Type()方法获取变量的reflect.Type。

如果要修改值，请记住它必须是一个指针，并且必须首先对其进行解引用。
使用refPtrVal.Elem().Set(newRefVal)来修改值，并且传递给Set()的值也必须是reflect.Value。
*/

package main

import (
	"fmt"
	"reflect"
)

type Foo struct {
	A int
	B string
}

// 使用反射创建新实例
func main() {
	greeting := "hello"
	f := Foo{A: 10, B: "Salutations"}

	gVal := reflect.ValueOf(greeting)
	// not a pointer so all we can do is read it
	fmt.Println(gVal.Interface()) // hello

	gpVal := reflect.ValueOf(&greeting)
	// it’s a pointer, so we can change it, and it changes the underlying variable
	gpVal.Elem().SetString("goodbye")
	fmt.Println(greeting) // 修改成了goodbye

	fType := reflect.TypeOf(f)
	fVal := reflect.New(fType)
	fVal.Elem().Field(0).SetInt(20)
	fVal.Elem().Field(1).SetString("Greetings")
	f2 := fVal.Elem().Interface().(Foo) // 调用Interface()方法从reflect.Value回到普通变量值
	f3 := fVal.Interface()              //f3: &{20 Greetings}
	f4 := fVal.Elem().Field(1).Interface()
	fmt.Printf("f2: %+v, %d, %s\n", f2, f2.A, f2.B)
	fmt.Println("f2:", f2)
	fmt.Println("f3:", f3)
	fmt.Println("f4:", f4)
	fmt.Println("f:", f)

	gret := "I like hh"
	v := reflect.ValueOf(&gret)
	fmt.Println(v.Elem().Interface()) // "I like hh"
	v.Elem().SetString("hh like me")
	fmt.Println(gret)

	v1 := reflect.ValueOf(gret)
	fmt.Println(v1.Interface())

	fmt.Println("----------------")
	ff := Foo{1, "tt"}
	vv := reflect.ValueOf(ff)
	fmt.Println(vv.Field(1).Interface()) //tt
	//sfmt.Println(vv.Elem().Field(1).Interface()) //错误的写法， 值类型不能用Elem()
}
