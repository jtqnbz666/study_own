package main

import "fmt"

type A interface {
	Print()
}

type B1 interface {
	A
	PrintB1()
}

type B2 interface {
	A
	PrintB2()
}

type CI interface {
	B1
	B2
	PrintC()
}

type C struct {
	Age int
}

func (p C) Print() { // 实现 Running方法
	fmt.Println("A:", p.Age)
}

func (p C) PrintB1() { // 实现 Running方法
	fmt.Println("B1:", p.Age)
}

func (p C) PrintB2() { // 实现 Running方法
	fmt.Println("B2:", p.Age)
}

func (p C) PrintC() { // 实现 Running方法
	fmt.Println("C:", p.Age)
}

func main() {
	var c C
	var ci CI
	ci = c
	ci.Print()
	ci.PrintB1()
}
