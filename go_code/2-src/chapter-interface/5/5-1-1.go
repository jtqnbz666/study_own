// 按字节遍历
package main

import "fmt"

func main() {
	var s = "嘻哈china"
	for i := 0; i < len(s); i++ {
		fmt.Printf("%x ", s[i])
	}

}
