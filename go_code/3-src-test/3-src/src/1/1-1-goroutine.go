package main

import (
	"fmt"
	"time"
)

func hello(name string) {
	for {
		fmt.Println("Hello World :", name)
		time.Sleep(1 * time.Second)
	}
}

func main() {
	go hello("Taoshuai!") 
	time.Sleep(5 * time.Second)
	fmt.Println("main function")
}