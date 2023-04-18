package main

import (
	"fmt"
	"strings"
	"unsafe"
	_ "unsafe"
)

func test1() {
	bytes := []byte("I am byte array !")
	str := string(bytes)
	bytes[0] = 'i' //注意这一行，bytes在这里修改了数据，但是str打印出来的依然没变化，
	fmt.Println(str)
}
func test2() {
	bytes := []byte("I am byte array !")
	str := (*string)(unsafe.Pointer(&bytes))
	bytes[0] = 'i'
	fmt.Println(*str)
}
func test3() {
	var data [10]byte
	data[0] = 'T'
	data[1] = 'E'
	var str string = string(data[:])
	fmt.Println(str)
}

func str2bytes(s string) []byte {
	x := (*[2]uintptr)(unsafe.Pointer(&s))
	h := [3]uintptr{x[0], x[1], x[1]}
	return *(*[]byte)(unsafe.Pointer(&h))
}

func bytes2str(b []byte) string {
	return *(*string)(unsafe.Pointer(&b))
}
func test4() {
	s := strings.Repeat("abc", 3)
	fmt.Println("str2bytes")
	b := str2bytes(s)
	fmt.Println("bytes2str")
	s2 := bytes2str(b)
	fmt.Println(b, s2)
}

func main() {
	test1()
	test2()
	test3()
	test4()
}
