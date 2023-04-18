package main

import (
	"fmt"
	"log"
	"net"
)

func handleConn(c net.Conn) {
	defer c.Close()

	//read from the connection
	var buf = make([]byte, 10)
	log.Println("start to read from conn")
	n, err := c.Read(buf)
	if err != nil {
		log.Println("conn read error:", err)
	} else {
		log.Printf("read %d bytes, content is %s\n", n, string(buf[:n]))
	}

	n, err = c.Write(buf)
	if err != nil {
		log.Println("conn write error:", err)
	} else {
		log.Printf("write %d bytes, content is %s\n", n, string(buf[:n]))
	}
}

func main() {
	listen, err := net.Listen("tcp", ":8888")
	if err != nil {
		fmt.Println("listen error:", err)
		return
	}

	for {
		conn, err := listen.Accept()
		if err != nil {
			fmt.Println("accept error:", err)
			break
		}
		str := "hello"
		var buf = make([]byte, 10)
		buf = []byte(str) //正确
		test := []string{"qwe"}
		fmt.Println(test) //这样是可以的
		test1 := []string{"hello", "world"}
		fmt.Println(test1)
		test3 := []byte{'1', '2'} //可以
		fmt.Println(test3)        //这样是可以的
		// test := []byte{"ttt"}
		// fmt.Println(test)			//这样是错误的
		// test := []byte("ttt")
		// fmt.Println(test)			//正确

		conn.Write(buf)
		go handleConn(conn)
	}
}
