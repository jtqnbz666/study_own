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

// 更新数据
func updateRowDemo(db *sql.DB) {
	sqlStr := "update user set age=? where id = ?"
	ret, err := db.Exec(sqlStr, 20, 2)
	if err != nil {
		fmt.Printf("update failed, err:%v\n", err)
		return
	}
	n, err := ret.RowsAffected() // 操作影响的行数
	if err != nil {
		fmt.Printf("get RowsAffected failed, err:%v\n", err)
		return
	}
	fmt.Printf("update success, affected rows:%d\n", n)
}
func main() {
	db, err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4")
	// fmt.Println("err:", err)
	err = db.Ping()
	if err != nil {
		fmt.Println("数据库链接失败", err)
		return
	}
	updateRowDemo(db)
	defer db.Close()
}
