package main

import (
	"fmt"
	"math"
)

type Circle struct {
	x      int
	y      int
	Radius int
}

// 面积
func (c Circle) Area() float64 {
	return math.Pi * float64(c.Radius) * float64(c.Radius)
}

// 周长
func (c Circle) Circumference() float64 {
	return 2 * math.Pi * float64(c.Radius)
}

func (c Circle) expand() {
	c.Radius *= 2
}

func (c *Circle) expand2() {
	c.Radius *= 2
}
func main() {
	var c = Circle{Radius: 50}
	fmt.Println(c.Area(), c.Circumference())
	// 指针变量调用方法形式上是一样的
	var pc = &c
	pc.expand2()
	fmt.Println(pc.Area(), pc.Circumference())

}
