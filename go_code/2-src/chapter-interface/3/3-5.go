// 3 reflect反射 基础
package main

import (
	"fmt"
)

func main() {
	newValue := make(map[interface{}]interface{}, 0)
	newValue[11] = "darren"
	newValue["age"] = 18
	fmt.Println(newValue)
}
