package main

import (
	"fmt"
	"log"

	"github.com/Unknwon/goconfig"
)

func main() {
	cfg, err := goconfig.LoadConfigFile("./conf.ini")	// 读取后文件关闭了
	if err != nil {
		log.Fatalf("无法加载配置文件：%s", err)
	}
	userListKey, err := cfg.GetValue("", "USER_LIST")
	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(userListKey)
	userListKey2, _ := cfg.GetValue(goconfig.DEFAULT_SECTION, "USER_LIST")
	fmt.Println(userListKey2)
	maxCount := cfg.MustInt("", "MAX_COUNT")
	fmt.Println(maxCount)
	maxPrice := cfg.MustFloat64("", "MAX_PRICE")
	fmt.Println(maxPrice)
	isShow := cfg.MustBool("", "IS_SHOW")
	fmt.Println(isShow)

	db := cfg.MustValue("test", "dbdns")
	fmt.Println(db)

	dbProd := cfg.MustValue("prod", "dbdns")
	fmt.Println("dbProd: ",dbProd)

	//set 值
	cfg.SetValue("", "MAX_NEW", "100")
	maxNew := cfg.MustInt("", "MAX_NEW")
	fmt.Println(maxNew)

	maxNew1, err := cfg.Int("", "MAX_NEW")
	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(maxNew1)
	cfg.AppendFiles("conf1.ini")
	// cfg.DeleteKey("", "MAX_NEW")
}
