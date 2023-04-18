package main

import (
	"fmt"
	"time"
)

func main() {
	go fmt.Println("hello go")
	go fmt.Println("hello 零声学院")
	time.Sleep(time.Second)
}
