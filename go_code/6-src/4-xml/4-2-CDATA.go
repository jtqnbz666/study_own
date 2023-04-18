package main

import (
	"encoding/xml"
	"fmt"
	"strconv"
	"time"

	"github.com/clbanning/mxj"
)

// tag中含有"-"的字段不会输出
// tag中含有"name,attr"，会以name作为属性名，字段值作为值输出为这个XML元素的属性，如上version字段所描述
// tag中含有",attr"，会以这个struct的字段名作为属性名输出为XML元素的属性，类似上一条，只是这个name默认是字段名了。
// tag中含有",chardata"，输出为xml的 character data而非element。
// tag中含有",innerxml"，将会被原样输出，而不会进行常规的编码过程
// tag中含有",comment"，将被当作xml注释来输出，而不会进行常规的编码过程，字段值中不能含有"--"字符串
// tag中含有"omitempty",如果该字段的值为空值那么该字段就不会被输出到XML，空值包括：false、0、nil指针或nil接口，任何长度为0的array, slice, map或者string

type CDATAText struct {
	Text string `xml:",innerxml"`
}

type Base struct {
	FromUserName CDATAText
	ToUserName   CDATAText
	MsgType      CDATAText
	CreateTime   CDATAText
}

// 文本消息的封装
type TextMessage struct {
	XMLName xml.Name `xml:"xml"`
	Base
	Content CDATAText
}

// 图片消息的封装
type PictureMessage struct {
	XMLName xml.Name `xml:"xml"`
	Base
	PicUrl  CDATAText
	MediaId CDATAText
}

func value2CDATA(v string) CDATAText {
	return CDATAText{"<![CDATA[" + v + "]]>"}
}

func main() {
	// 1. 解析 XML
	xmlStr := `<xml> 
		<ToUserName><![CDATA[toUser]]></ToUserName> 
		<FromUserName><![CDATA[fromUser]]></FromUserName> 
		<CreateTime>1348831860</CreateTime> 
		<MsgType><![CDATA[text]]></MsgType> 
		<Content><![CDATA[this is a test]]></Content> 
		<MsgId>1234567890123456</MsgId> 
		</xml>`

	var Message map[string]interface{}
	m, err := mxj.NewMapXml([]byte(xmlStr)) //使用了第三方的库

	if err != nil {
		return
	}

	if _, ok := m["xml"]; !ok {
		fmt.Println("Invalid Message.")
		return
	}
	fmt.Println("-->m:", m)
	message, ok := m["xml"].(map[string]interface{}) // 把xml对应的值读取出来

	if !ok {
		fmt.Println("Invalid Field `xml` Type.")
		return
	}

	Message = message

	fmt.Println("1. 解析出来:", Message) // xml对应的字段还是在map

	// 2. 封装XML
	var reply TextMessage
	inMsg, ok := Message["Content"].(string) // 读取内容 .(string)转成什么类型的数据

	if !ok {
		return
	}
	fmt.Println("Message[ToUserName].(string):", Message["ToUserName"].(string)) // 如果服务器要处理

	// 封装回复消息，需要添加 CDATA
	reply.Base.FromUserName = value2CDATA(Message["ToUserName"].(string))
	reply.Base.ToUserName = value2CDATA(Message["FromUserName"].(string))
	reply.Base.CreateTime = value2CDATA(strconv.FormatInt(time.Now().Unix(), 10))
	reply.Base.MsgType = value2CDATA("text")
	reply.Content = value2CDATA(fmt.Sprintf("我收到的是：%s", inMsg))

	replyXml, err := xml.Marshal(reply)                // 得到的是byte
	fmt.Println("2. 生成XML:", string(replyXml))         // []byte -> string
	fmt.Println("2. 生成XML:", []byte(string(replyXml))) // string -> []byte
}
