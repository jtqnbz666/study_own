package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	//设置连接模式 ， ip和端口号
	conn, err := net.Dial("tcp", "127.0.0.1:8888")
	if err != nil {
		fmt.Println("client dial err=", err)
		return
	}
	defer conn.Close()
	// 在命令行输入单行数据
	reader := bufio.NewReader(os.Stdin)
	for {
		//从终端读取一行用户的输入，并发给服务器
		line, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("readString err=", err)
		}
		//去掉输入后的换行符
		line = strings.Trim(line, "\r\n")
		//如果是exit,则退出客户端
		if line == "exit" {
			fmt.Println("客户端退出了")
			break
		}
		//将line发送给服务器
		n, e := conn.Write([]byte(line))
		if e != nil {
			fmt.Println("conn.write err=", e)
		}
		fmt.Printf("客户端发送了%d字节的数据\n", n)
	}
}
