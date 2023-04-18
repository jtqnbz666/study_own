package main 

import(
	"fmt"
	"log"
	"github.com/Unknown/goconfig"
)

func main() {
	cfg, err := goconfig.LoadConfigFile("./conf.ini")//读取后文件关闭了
}