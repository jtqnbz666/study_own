package main

import (
	"my_gin"
	"net/http"
)

func main() {
	r := my_gin.Default()
	r.GET("/", func(c *my_gin.Context) {
		c.String(http.StatusOK, "Hello jt\n")
	})
	//故意引起错误. 索引越界
	r.GET("/panic", func(c *my_gin.Context) {
		names := []string{"jtdashuaige"}
		c.String(http.StatusOK, names[100])
	})

	r.Run(":8080")
}
