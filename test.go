package main

import (
	"fmt"
	"time"
)

func main() {
	unixTime := time.Now().Add(30 * 24 * time.Hour).Unix()
	fmt.Println(unixTime)
}
