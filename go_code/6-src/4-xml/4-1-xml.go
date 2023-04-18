package main

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"os"
)

// 如果struct中有一个叫做XMLName，且类型为xml.Name字段，
// 那么在解析的时候就会保存这个element的名字到该字段, 比如这里的config
type SConfig struct {
	XMLName      xml.Name   `xml:"config"`     // 指定最外层的标签为config
	SmtpServer   string     `xml:"smtpServer"` // 读取smtpServer配置项，并将结果保存到SmtpServer变量中
	SmtpPort     int        `xml:"smtpPort"`
	Sender       string     `xml:"sender"`
	SenderPasswd string     `xml:"senderPasswd"`
	Receivers    SReceivers `xml:"receivers"` // 读取receivers标签下的内容，以结构方式获取
}

type SReceivers struct {
	Age    int      `xml:"age"`
	Flag   string   `xml:"flag,attr"` // 读取flag属性
	User   []string `xml:"user"`      // 读取user数组
	Script string   `xml:"script"`    // 读取 <![CDATA[ xxx ]]> 数据
}

func main() {
	file, err := os.Open("4-1-xml.xml") // For read access.
	if err != nil {
		fmt.Printf("error: %v", err)
		return
	}
	defer file.Close()
	data, err := ioutil.ReadAll(file)
	if err != nil {
		fmt.Printf("error: %v", err)
		return
	}
	v := SConfig{}
	err = xml.Unmarshal(data, &v) // 反序列化
	if err != nil {
		fmt.Printf("error: %v", err)
		return
	}

	fmt.Println("文本:", v)
	fmt.Println("解析结果:")
	fmt.Println("XMLName : ", v.XMLName)
	fmt.Println("SmtpServer : ", v.SmtpServer)
	fmt.Println("SmtpPort : ", v.SmtpPort)
	fmt.Println("Sender : ", v.Sender)
	fmt.Println("SenderPasswd : ", v.SenderPasswd)
	fmt.Println("Receivers.Flag : ", v.Receivers.Flag)
	for i, element := range v.Receivers.User {
		fmt.Println(i, element)
	}
}
