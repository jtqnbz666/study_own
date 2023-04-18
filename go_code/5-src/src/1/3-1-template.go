package main

import (
	"fmt"
	"html/template"
	"io"
	"net/http"
)

var myTemplate *template.Template

type Result struct {
	output string
}

func (p *Result) Write(b []byte) (n int, err error) {
	fmt.Println("called by template")
	p.output += string(b)
	return len(b), nil
}

type Person struct {
	Name  string
	Title string
	Age   int
}

func userInfo(w http.ResponseWriter, r *http.Request) {
	fmt.Println("handle hello")
	//fmt.Fprintf(w, "hello ")
	var arr []Person
	p := Person{Name: "Darren", Age: 18, Title: "我的个人网站"}
	p1 := Person{Name: "King", Age: 19, Title: "我的个人网站"}
	p2 := Person{Name: "柚子", Age: 20, Title: "我的个人网站"}
	arr = append(arr, p)
	arr = append(arr, p1)
	arr = append(arr, p2)

	fmt.Println("arr:", arr)

	resultWriter := &Result{}
	io.WriteString(resultWriter, "hello 模板")
	err := myTemplate.Execute(w, arr) // 模板替换, 执行完后， html模板和参数arr就写入 w http.ResponseWriter
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println("template render data:", resultWriter.output)
	//myTemplate.Execute(w, p)
	//myTemplate.Execute(os.Stdout, p)
	//file, err := os.OpenFile("C:/test.log", os.O_CREATE|os.O_WRONLY, 0755)
	//if err != nil {
	//	fmt.Println("open failed err:", err)
	//	return
	//}

}

func initTemplate(filename string) (err error) {
	myTemplate, err = template.ParseFiles(filename)
	if err != nil {
		fmt.Println("parse file err:", err)
		return
	}
	return
}

func main() {
	initTemplate("./index.html")
	http.HandleFunc("/user/info", userInfo)
	err := http.ListenAndServe("0.0.0.0:9000", nil)
	if err != nil {
		fmt.Println("http listen failed")
	}
}
