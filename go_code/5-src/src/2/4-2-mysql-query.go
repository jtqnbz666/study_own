package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

type user struct {
	id   int
	name string
	age  int
}

// 查询单条数据示例
func queryRowDemo(db *sql.DB) {
	sqlStr := "select id, name, age from user where id=?"
	var u user
	// 非常重要：确保QueryRow之后调用Scan方法，否则持有的数据库链接不会被释放
	err := db.QueryRow(sqlStr, 3).Scan(&u.id, &u.name, &u.age)
	if err != nil {
		fmt.Printf("scan failed, err:%v\n", err)
		return
	}
	fmt.Printf("id:%d name:%s age:%d\n", u.id, u.name, u.age)
}
func main() {
	db, err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4")
	fmt.Println("err:", err)
	err = db.Ping()
	if err != nil {
		fmt.Println("数据库链接失败", err)
		return
	}
	queryRowDemo(db)
	defer db.Close()
}
