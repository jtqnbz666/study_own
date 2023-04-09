package main

import(
	"reflect"
	"fmt"
)

type student struct {
Name string 
Age int
}

func main() {

//a := student {
//	Name : "jt",
//	Age : 18,
//	phone: "110",
//}

//tmp := reflect.TypeOf(&a)
//fmt.Println(tmp.Elem().Name())

//typ := reflect.TypeOf(a)
//val := reflect.New(typ).Elem()
//val := reflect.ValueOf(&a).Elem()
//val.FieldByName("Name").SetString("jtsg")// 正确
// val.FieldByName("Age").SetInt(20)// 正确
//tp := val.Interface().(student)
//fmt.Println(tp.Name, tp.Age)
//fmt.Println(val.Field(0))
//fmt.Println(val.Field(1).Interface())
//fmt.Println(a.Name, a.Age)
//fmt.Println(val.Elem().Field(0))//SetString("tt")

//fmt.Println(val.Elem().Field(1).SetInt(20)) //错误
//f1 := val.Elem().Interface().(student)
//fmt.Println(f1.Name, f1.Age)

//fmt.Println(val.Elem().Field(0).Interface().(string))
/*
gre := "hello "
tt := reflect.ValueOf(&gre)
tt.Elem().SetString("hh")
fmt.Println(gre)
fmt.Println(tt.Elem().Interface())
*/
//v := reflect.ValueOf(&a)
//fmt.Println(v.Elem().Field(1).Interface())
//fmt.Println(v.Field(0))

type MyInt int
var x MyInt = 5
v := reflect.TypeOf(x)
fmt.Println(v.Kind())
fmt.Println(v.Name())

}
