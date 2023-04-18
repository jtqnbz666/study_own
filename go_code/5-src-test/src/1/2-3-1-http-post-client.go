package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
)

func main() {
	url := "http://127.0.0.1:9000/post"
	contentType := "appplication/json"
	data := `{"name":"jt", "age":18}` //body
	resp, err := http.Post(url, contentType, strings.NewReader(data))
	if err != nil {
		fmt.Println("post failed, err:%v\n", err)
		return
	}
	defer resp.Body.Close()
	b, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("get resp failed, err:%v\n", err)
		return
	}
	fmt.Println("StatusCode:", resp.StatusCode)
	fmt.Println(string(b))
}
