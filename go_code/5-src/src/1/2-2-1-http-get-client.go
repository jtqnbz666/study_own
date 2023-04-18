package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	// resp, err := http.Get("http://127.0.0.1:9000")
	resp, err := http.Get("https://www.baidu.com/")
	if err != nil {
		fmt.Println("get err:", err)
		return
	}
	defer resp.Body.Close() // 做关闭
	// data byte
	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("get data err:", err)
		return
	}

	fmt.Println("body:", string(data))
	fmt.Println("resp:", resp)
}
