package main

import("fmt")

func main() {
var str  = "%shha%s"
str = fmt.Sprintf(str, "1", "2", 3)
fmt.Println(str)
}
