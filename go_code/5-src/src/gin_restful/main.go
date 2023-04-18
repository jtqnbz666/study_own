package main

import "gin_http/db"

// go mod init  xx_project
// go build
// ./xx_project
func main() {
	defer db.SqlDB.Close()
	router := initRouter()
	router.Run(":8806") // 启动服务了
}
