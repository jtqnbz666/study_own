package main

import (
	"fmt"
	"log"
	"net/http"
)

type Engine struct {
}

func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	switch req.URL.Path {
	case "/":
		fmt.Fprintf(w, "URL.Path = %q\n", req.URL.Path)
	case "/hello":
		for k, v := range req.Header {
			fmt.Fprintf(w, "Header[%q] = %q\n", k, v)
		}
	default:
		fmt.Fprintf(w, "404 NOT FOUND:%s\n", req.URL)
	}
}

func main() {
	engine := new(Engine)

	log.Fatal(http.ListenAndServe(":9999", engine))
	//这里如果使用nil， 就必须使用http.HandleFunc("/", indexHandler) 来注册处理函数
	//如果是engin 就需要给engine实现ServeHTTP方法
}
