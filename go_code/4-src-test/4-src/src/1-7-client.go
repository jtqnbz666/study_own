package main

import(
	"log"
	"net"
	"os"
	"time"
)

func main() {
	connTimeout := 3 * time.Second 
	conn, err := net.DialTimeout("tcp", "127.0.0.1:8080", connTimeout)
	if err != nil {
		log.Println("dial failed:", err)
		os.Exit(1)
	}
	defer conn.Close()
	readTimeout := 2 * time.Second 
	buffer := make([]byte, 512)

	for {
		err = conn.SetReadDeadline(time.Now().Add(readTimeout))
		if err != nil {
			log.Println("setReadDeadline failed:", err)
		}
		n, err := conn.Read(buffer)
		if err != nil {
			log.Println("Read failed:", err)
		}
		
		log.Println("count:", n, "msg:",string(buffer))
	}
}