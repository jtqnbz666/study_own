package main

import(
	"log"
	"net"
	"time"
)


func main () {
	addr := "0.0.0.0:8080"

	tcpAddr, err := net.ResolveTCPAddr("tcp", addr)

	if err != nil {
		log.Fatalf("net.ResovleTCPAddr fail:%s", addr)
	}
	listenner, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		log.Fatalf("listen %s fail: %s", addr, err)
	} else {
		log.Println("listening", addr)
	}

	for {
		conn, err := listenner.Accept()
		if err != nil {
			log.Println("listener.Accept error:", err)
			continue 
		}

		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	
	var buffer []byte = []byte("You are welcome. I'm server.")

	for {
		time.Sleep(3 * time.Second)
		n, err := conn.Write(buffer)
		if err != nil {
			log.Println("Write error:", err)
			break
		}
		log.Println("send:", n)
	}

	log.Println("connection end")
}