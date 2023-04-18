package main

import (
	"fmt"
	"net/http"
)

func sayHello(w http.ResponseWriter, r *http.Request) {
	_, _ = w.Write([]byte("您好!"))
}
func helloT(w http.ResponseWriter, r *http.Request) {
	_, _ = w.Write([]byte("您好JT!"))
}
func helloH(w http.ResponseWriter, r *http.Request) {
	_, _ = w.Write([]byte("您好WH!"))
}
func main() {
	http.HandleFunc("/", sayHello)
	http.HandleFunc("/dd", helloT)
	http.HandleFunc("/kk", helloH)

	//设置监听的TCP地址并启动服务
	//参数一： TCP地址(IP + Port)
	//参数二： handle handle 参数一般为nil， 此时会使用默认DefaultServeMux
	err := http.ListenAndServe("0.0.0.0:9000", nil)
	if err != nil {
		fmt.Printf("执行错误, 错误为：%v\n", err)
		return
	}
}
