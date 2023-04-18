package main

import "fmt"

func calc(a, b int) (sum int, avg int) {
	sum = a + b
	avg = (a + b) / 2
	return
}

func main() {
	sum, avg := calc(10, 20)
	fmt.Println("sum: ", sum, ", avg: ", avg)
}
