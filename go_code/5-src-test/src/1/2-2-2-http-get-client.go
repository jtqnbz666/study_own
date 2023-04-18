package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
)

func main() {
	//处理请求参数
	params := url.Values{}
	params.Set("name", "jt")
	params.Set("hobby", "游戏")

	//2.设置请求URL
	rawUrl := "http://127.0.0.1:9000"
	reqURL, err := url.ParseRequestURI(rawUrl)
	if err != nil {
		fmt.Printf("执行错误，错误为：%v", err)
		return
	}
	fmt.Println("reqURL:", reqURL) //reqURL: http://127.0.0.1:9000
	//整合请求URL和参数
	//Encode方法将请求参数编码为url编码格式("bar=baz&foo=quux")，编码时会以键进行排序。)
	reqURL.RawQuery = params.Encode()

	//发送HTTP请求
	//说明：reqURL.String() String将URL重构为一个合法的URL字符串
	fmt.Println("Get url:", reqURL.String()) //http://127.0.0.1:9000/?hobby=%E6%B8%B8%E6%88%8F&name=jt
	resp, err := http.Get(reqURL.String())
	if err != nil {
		fmt.Printf("执行错误, 错误为：%v", err)
		return
	}
	defer resp.Body.Close()

	//一次性读取相应的所有内容
	body, err := ioutil.ReadAll(resp.Body)

	if err != nil {
		fmt.Printf("执行错误， 错误为:%v", err)
		return
	}
	fmt.Println("Response:", string(body)) //Response: name: jt hobby: 游戏
}
