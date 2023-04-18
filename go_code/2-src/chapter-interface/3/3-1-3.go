package main

import (
	"fmt"
	"reflect"
)

func main() {
	str := "darren"
	val := reflect.ValueOf(str).Kind()
	fmt.Println(val) //string
}
