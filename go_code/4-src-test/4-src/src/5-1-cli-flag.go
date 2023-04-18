package main

import (
	"flag"
	"fmt"
	//    "os"
)

// go run 4-1-cli-flag.go -ok -id 11111 -port 8899 -name TestUser very goo
func main() {
	//    fmt.Println(os.Args)
	ok := flag.Bool("ok", false, "is ok")		// 不设置ok 则为false
	id := flag.Int("id", 0, "id")
	port := flag.String("port", ":8080", "http listen port")
	var name string
	flag.StringVar(&name, "name", "Jack", "name")

	flag.Parse()
	//    flag.Usage()
	others := flag.Args()

	fmt.Println("ok:", *ok)
	fmt.Println("id:", *id)
	fmt.Println("port:", *port)
	fmt.Println("name:", name)
	fmt.Println("other:", others)
}
