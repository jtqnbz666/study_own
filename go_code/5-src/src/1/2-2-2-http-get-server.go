package main

import (
	"fmt"
	"net/http"
)
// 响应: http.ResponseWriter
// 请求:http.Request 
func myHandler(w http.ResponseWriter, r *http.Request) {
	defer r.Body.Close()
	params := r.URL.Query()
	fmt.Println("r.URL: ", r.URL)
	fmt.Fprintln(w, "name:", params.Get("name"), "hobby:", params.Get("hobby")) // 回写数据
}
func main() {

	http.HandleFunc("/", myHandler)
	err := http.ListenAndServe("127.0.0.1:9000", nil)
	if err != nil {
		fmt.Printf("http.ListenAndServe()函数执行错误,错误为:%v\n", err)
		return
	}
}
