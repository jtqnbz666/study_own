package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql" // 注释掉后异常 _ 调用初始化函数
)

// https://github.com/go-sql-driver/mysql#usage
func main() {
	db, err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4")
	fmt.Println("err:", err) // err: <nil>
	if db == nil {
		fmt.Println("db open failed:", err)
	}

	err = db.Ping() //Ping verifies a connection to the database is still alive, establishing a connection if necessary
	if err != nil {
		fmt.Println("数据库链接失败", err)
	}
	defer db.Close()
}
