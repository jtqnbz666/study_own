package main

import (
	"fmt"
	"io"
	"net/http"
)

const form = `<html><body><form action="#" method="post" name="bar">
                    <input type="text" name="in"/>
                    <input type="text" name="in"/>
                     <input type="submit" value="Submit"/>
             </form></html></body>`

func HomeServer(w http.ResponseWriter, request *http.Request) {
	io.WriteString(w, "/test1 或者/test2")
	// io.WriteString(w, "<h1>/test1 或者/test2</h1>")
}

func SimpleServer(w http.ResponseWriter, request *http.Request) {
	io.WriteString(w, "<h1>hello, world</h1>")
}

func FormServer(w http.ResponseWriter, request *http.Request) {
	w.Header().Set("Content-Type", "text/html")
	switch request.Method {
	case "GET":
		io.WriteString(w, form)
	case "POST":
		request.ParseForm()
		fmt.Println("request.Form[in]:", request.Form["in"])
		io.WriteString(w, request.Form["in"][0])
		io.WriteString(w, "\n")
		io.WriteString(w, request.Form["in"][1])	// go web开发
		// var ptr *int
		// *ptr = 0x123445 // 模拟异常
	}
}
func main() {
	http.HandleFunc("/", HomeServer)
	http.HandleFunc("/test1", SimpleServer)
	http.HandleFunc("/test2", FormServer)
	err := http.ListenAndServe(":9000", nil)
	if err != nil {
		fmt.Printf("http.ListenAndServe()函数执行错误,错误为:%v\n", err)
		return
	}
}
