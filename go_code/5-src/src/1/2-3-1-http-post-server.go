package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func postHandler(w http.ResponseWriter, r *http.Request) {
	defer r.Body.Close()
	fmt.Println("Method ", r.Method)
	if r.Method == "POST" {
		// 1. 请求类型是application/json时从r.Body读取数据
		b, err := ioutil.ReadAll(r.Body)
		if err != nil {
			fmt.Println("read request.Body failed, err:%v\n", err)
			return
		}
		fmt.Println(string(b))
		answer := `{"status": "ok"}`
		w.Write([]byte(answer))
	} else {
		fmt.Println("can't handle ", r.Method)
		w.WriteHeader(http.StatusBadRequest)
	}

}
func main() {

	http.HandleFunc("/post", postHandler)
	err := http.ListenAndServe("0.0.0.0:9000", nil)
	if err != nil {
		fmt.Printf("http.ListenAndServe()函数执行错误,错误为:%v\n", err)
		return
	}
}
