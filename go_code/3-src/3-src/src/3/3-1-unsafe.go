package main

import "fmt"
// go多协程 是有竞态，不像以前的ntyco，libco没有竞态的
func write(d map[string]int) {
	d["fruit"] = 2
}

func read(d map[string]int) {
	fmt.Println(d["fruit"])
}

// go run -race 3-1-unsafe.go
func main() {
	d := map[string]int{}
	go read(d)
	write(d)
}
