package main

import (
	"fmt"
	"net"
	_ "time"
)

func process(conn net.Conn) {
	//这里接受客户端的数据
	defer conn.Close()
	for {
		//创建一个新的切片
		buf := make([]byte, 1024)
		//等待客户端发送信息，如果客户端没发送，协程就阻塞在这
		// fmt.Printf("服务器在等待客户端%v的输入\n", conn.RemoteAddr().String())
		// conn.SetReadDeadline(time.Now().Add(time.Duration(1) * time.Second))
		n, err := conn.Read(buf) // 默认是阻塞的
		if err != nil {
			fmt.Println("服务器read err=", err)
			fmt.Println("客户端退出了")
			return
		}
		//显示客户端发送内容到服务器的终端
		fmt.Print(string(buf[:n]) + "\n")

	}
}

func main() {
	fmt.Println("服务器开始监听...")
	//协议、端口
	listen, err := net.Listen("tcp", "0.0.0.0:8888")
	if err != nil {
		fmt.Println("监听失败,err=", err)
		return
	}
	//延时关闭
	defer listen.Close() // 函数退出的时候调用
	for {
		//循环等待客户端连接
		fmt.Println("等待客户端连接...")
		conn, err := listen.Accept()
		if err != nil {
			fmt.Println("Accept() err=", err)
		} else {
			fmt.Printf("Accept() suc con=%v,客户端Ip=%v\n", conn, conn.RemoteAddr().String())
		}
		//这里准备起个协程为客户端服务
		go process(conn)
	}
	//fmt.Printf("监听成功，suv=%v\n", listen)
}
