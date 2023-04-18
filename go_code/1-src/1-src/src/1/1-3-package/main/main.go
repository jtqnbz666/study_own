package main

import (
	"1/1-3-package/calc"
	"fmt"
)

func main() {
	sum := calc.Add(100, 300)
	sub := calc.Sub(100, 300)

	fmt.Println("sum=", sum)
	fmt.Println("sub=", sub)

	sum, avg := calc.Calc(100, 300)
	fmt.Println("sum=", sum)
	fmt.Println("avg=", avg)
}
