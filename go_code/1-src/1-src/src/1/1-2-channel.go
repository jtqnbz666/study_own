package main

import "fmt"

func test_pipe() {
	pipe := make(chan int, 3)
	pipe <- 1
	pipe <- 2
	pipe <- 3
	var t1 int
	t1 = <-pipe
	fmt.Println("t1: ", t1)

}

func sum(s []int, c chan int) {
	test_pipe()
	sum := 0
	for _, v := range s {
		sum += v
	}
	fmt.Println("sum:", sum)
	c <- sum // send sum to c
}

func main() {
	s := []int{7, 2, 8, -9, 4, 0}
	c := make(chan int)
	go sum(s[:len(s)/2], c) // 7+2+8 = 17, -9 + 4+0 = -5
	go sum(s[len(s)/2:], c)
	// x, y := <-c, <-c // receive from c
	x := <-c
	y := <-c
	fmt.Println(x, y, x+y)
}
