package main

import(
	"bytes"
	"fmt"
	"io"
	"sync"
)

var bufPool sync.Pool 

//预定义定界符
const delimiter = '\n'

//一个简易的数据库缓冲区的接口
type Buffer interface {
	Delimiter() byte 			//获取数据块之间的定界符
	Write(contents string) (err error) //写入一个数据块
	Read() (contents string, err error) //读取一个数据块
	Free()						//释放当前的缓冲区
}
//实现一个上边定义的接口
type myBuffer struct {
	buf			bytes.Buffer
	delimiter   byte 
}

func (b *myBuffer) Delimiter() byte {
	return b.delimiter
}

func (b *myBuffer)Write(contents string) (err error) {
	if _, err = b.buf.WriteString(contents); err != nil {
		return 
	}
	return b.buf.WriteByte(b.delimiter)
}

func (b *myBuffer) Read() (contents string, err error) {
	return b.buf.ReadString(b.delimiter)
}

func (b *myBuffer) Free() {
	bufPool.Put(b)
}

func init() {
	bufPool = sync.Pool {
		New: func() interface{} {
			return &myBuffer{delimiter: delimiter}
		},
	}
}

//获取一个数据库缓冲区
func GetBuffer() Buffer {
	return bufPool.Get().(Buffer)//接口转换数据类型， 回忆刚学接口的内容那里
}

func main() {
	buf := GetBuffer() 
	defer buf.Free() 
	buf.Write("写入第一行")
	buf.Write("写入第二行")
	fmt.Println("数据已经写入， 准备把数据读出")
	for {
		block, err := buf.Read() 
		if err != nil {
			if err == io.EOF {
				break 
			}
			panic(fmt.Errorf("读取缓冲区时ERROR：%s", err))
		}
		fmt.Print(block)
	}
}