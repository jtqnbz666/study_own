package main

import (
	"fmt"
)

func main() {
	str := "hello world,中国"
	for i, v := range str {
		fmt.Printf("index[%d] val[%c]\n", i, v)
	}
}
