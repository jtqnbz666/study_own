package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql" //这个_不能注释，里面对db做了init操作
)

func main() {
	db, err := sql.Open("mysql", "root:nswdsm58@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4")
	fmt.Println("err:", err)
	//不管是否成功err的返回值都是nil
	if db == nil {
		fmt.Println("db open faild:", err)
	}

	err = db.Ping() //Ping verifies a connection to the database is still alive, establishing a connection if necessary
	if err != nil {
		fmt.Println("数据库链接失败", err)
	}
	defer db.Close()
}
