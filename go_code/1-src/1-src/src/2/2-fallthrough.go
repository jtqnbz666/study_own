package main

import (
	"fmt"
)

func test_switch1() {
	a := 2
	switch a {
	case 1:
		fmt.Println("a=1")
	case 2:
		fmt.Println("a=2")
	case 3:
		fmt.Println("a=3")
	case 4:
		fmt.Println("a=4")
	default:
		fmt.Println("default")
	}
}

func test_switch2() {
	a := 2
	switch a {
	case 1:
		fmt.Println("a=1")
	case 2:
		fmt.Println("a=2")
		fallthrough
	case 3:
		fmt.Println("a=3")
	case 4:
		fmt.Println("a=4")
	default:
		fmt.Println("default")
	}
}

func main() {
	fmt.Printf("执行test_switch%d\n", 1)
	test_switch1()
	fmt.Printf("执行test_switch%d\n", 2)
	test_switch2()
}
