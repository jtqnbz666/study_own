package main

import (
	"my_gin"
	"net/http"
)

//curl -d 'username=jt&password=123' 127.0.0.1:8080/login
func main() {
	r := my_gin.New()
	r.GET("/", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "<h1>Hello my_gin</h1>")
	})
	//c.Query是针对于URL
	r.GET("/HELLO", func(c *my_gin.Context) {
		c.String(http.StatusOK, "hello %s, you are at %s\n", c.Query("name"), c.Path)
	})
	//c.PostForm是针对于body
	r.POST("/login", func(c *my_gin.Context) {
		c.JSON(http.StatusOK, my_gin.H{
			"username": c.PostForm("username"),
			"password": c.PostForm("password"),
		})
	})

	r.Run(":8080")

}
