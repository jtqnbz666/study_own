/*
使用反射创建函数
反射不仅仅可以为存储数据创造新的地方。还可以使用reflect.MakeFunc函数使用reflect来创建新函数。
该函数期望我们要创建的函数的reflect.Type，以及一个闭包，其输入参数为[]reflect.Value类型，
其返回类型也为[] reflect.Value类型。下面是一个简单的示例，它为传递给它的任何函数创建一个定时包装器
*/
package main

import (
	"fmt"
	"reflect"
	"runtime"
	"time"
)

func MakeTimedFunction(f interface{}) interface{} {
	rf := reflect.TypeOf(f)
	fmt.Println("rf: ", rf)
	if rf.Kind() != reflect.Func {
		panic("expects a function")
	}
	vf := reflect.ValueOf(f)
	fmt.Println("vf: ", vf)
	wrapperF := reflect.MakeFunc(rf, func(in []reflect.Value) []reflect.Value {
		start := time.Now()
		out := vf.Call(in)
		end := time.Now()
		fmt.Printf("calling %s took %v\n", runtime.FuncForPC(vf.Pointer()).Name(), end.Sub(start))
		return out
	})
	return wrapperF.Interface()
}

func timeMe() {
	fmt.Println("starting")
	time.Sleep(1 * time.Second)
	fmt.Println("ending")
}

func timeMeToo(a int) int {
	fmt.Println("starting")
	time.Sleep(time.Duration(a) * time.Second)
	result := a * 2
	fmt.Println("ending")
	return result
}

func main() {

	fmt.Println("MakeTimedFunction1: ")
	timed := MakeTimedFunction(timeMe).(func())
	fmt.Println("转成普通函数1: ")
	timed()

	fmt.Println("\n\nMakeTimedFunction2: ")
	timedToo := MakeTimedFunction(timeMeToo).(func(int) int)
	fmt.Println("转成普通函数2: ")
	fmt.Println(timedToo(5))
}
