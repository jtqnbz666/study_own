package main

import (
	"fmt"
	"net/http"
)

// say hello to the world
func sayHello(w http.ResponseWriter, r *http.Request) {
	//n, err := fmt.Fprintln(w, "hello world")
	_, _ = w.Write([]byte("你好!"))
}

func helloDarren(w http.ResponseWriter, r *http.Request) {
	//n, err := fmt.Fprintln(w, "hello world")
	_, _ = w.Write([]byte("你好,!"))
}
func main() {

	//1.注册一个处理器函数
	serveMux := http.NewServeMux()
	serveMux.HandleFunc("/", sayHello)
	serveMux.HandleFunc("/dd", helloDarren) // 其他url处理
	//2.设置监听的TCP地址并启动服务
	//参数1:TCP地址(IP+Port)
	//参数2:handler 创建新的*serveMux,不使用默认的
	err := http.ListenAndServe("0.0.0.0:9000", serveMux)
	if err != nil {
		fmt.Printf("http.ListenAndServe()函数执行错误,错误为:%v\n", err)
		return
	}
}
