package main

import (
	"flag"
	"fmt"
)

type FlagSet struct {
	Usage func()
}

var myFlagSet = flag.NewFlagSet("myflagset", flag.ExitOnError)
var stringFlag = myFlagSet.String("abc", "default value", "help mesage")

func main() {
	myFlagSet.Parse([]string{"-abc", "def", "ghi", "123"})
	args := myFlagSet.Args()
	for i := range args {
		fmt.Println(i, myFlagSet.Arg(i))
	}
}
