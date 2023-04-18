// 按字符 rune 遍历
package main

import "fmt"

func splice() {
	var s1 = "hello" // 静态字面量
	var s2 = ""
	for i := 0; i < 10; i++ {
		s2 += s1 // 动态构造
	}
	fmt.Println(len(s1))
	fmt.Println(len(s2))
}

// 字符串是只读的
// func onlyread() {
// 	var s = "hello"
// 	s[0] = 'H'
// }

// 切割切割
func cut() {
	var s1 = "hello world"
	var s2 = s1[3:8]
	fmt.Println(s2)
}

// 字节切片和字符串的相互转换
func string2bytes() {
	var s1 = "hello world"
	var b = []byte(s1) // 字符串转字节切片
	var s2 = string(b) // 字节切片转字符串
	fmt.Println(b)     //[104 101 108 108 111 32 119 111 114 108 100]
	fmt.Println(s2)    //hello world
}
func main() {
	splice()
	//onlyread()
	cut()
	string2bytes()
}
