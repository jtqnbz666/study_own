package main

import "fmt"

type A struct {
	Age int
}

type B1 struct {
	Age int
	a   A
}

type B2 struct {
	Age int
	a   A
}

type C struct {
	b1 B1
	b2 B2
	Age int
}

func (p A) Print() { // 实现 Running方法
	fmt.Println("A:", p.Age)
}

func (p B1) Print() { // 实现 Running方法
	fmt.Println("B1:", p.Age)
}

func (p B2) Print() { // 实现 Running方法
	fmt.Println("B2:", p.Age)
}

func (p C) Print() { // 实现 Running方法
	fmt.Println("C:", p.Age)
}

func main() {
	var c C
	c.Print()
	c.b1.Print()
	c.b2.Print()
}
