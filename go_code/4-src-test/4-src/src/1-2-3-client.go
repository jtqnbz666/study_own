package main

import(
	"log"
	"net"
	"time"
)

func main() {
	log.Println("begin dial ...")
	conn, err := net.DialTimeout("tcp", "127.0.0.1:8888", 2 * time.Second)
	if err != nil {
		log.Println("dial error:", err)
		return 
	}
	defer conn.Close()
	log.Println("dial ok")
}