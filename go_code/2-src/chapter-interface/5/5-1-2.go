// 按字符 rune 遍历
package main

import "fmt"

func main() {
	var s = "嘻哈china"
	for codepoint, runeValue := range s {
		fmt.Printf("[%d]: %x", codepoint, int32(runeValue))
	}
}
