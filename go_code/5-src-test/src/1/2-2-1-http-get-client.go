package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	resp, err := http.Get("https://www.baidu.com/")
	if err != nil {
		fmt.Println("get err:", err)
		return
	}
	defer resp.Body.Close() //关闭
	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("get data error:", err)
		return
	}
	fmt.Println("body:", string(data))
	fmt.Println("resp:", resp)
}
