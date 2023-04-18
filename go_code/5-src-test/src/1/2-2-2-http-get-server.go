package main

import (
	"fmt"
	"net/http"
)

//响应. http.ResponseWrite
//请求: http.Request

func myHandler(w http.ResponseWriter, r *http.Request) {
	defer r.Body.Close()
	params := r.URL.Query()
	fmt.Println("r.URL:", r.URL) ///?hobby=%E6%B8%B8%E6%88%8F&name=jt
	fmt.Fprintln(w, "name:", params.Get("name"), "hobby:", params.Get("hobby"))
}

func main() {
	http.HandleFunc("/", myHandler)
	err := http.ListenAndServe("127.0.0.1:9000", nil)
	if err != nil {
		fmt.Printf("执行错误：%v\n", err)
		return
	}
}
