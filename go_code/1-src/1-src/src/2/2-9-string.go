package main

import "fmt"

func main() {
	var str = "hello world\n\n"
	var str2 = `hello \n \n \n
	this is a test string
	This is a test string too·`
	fmt.Println("str=", str)
	fmt.Println("str2=", str2)
}
