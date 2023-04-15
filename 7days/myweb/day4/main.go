package main

import (
	"my_gin"
	"net/http"
)

func main() {
	r := my_gin.New()
	r.GET("/index", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "<h1>Inde Page</h1>")
	})
	v1 := r.Group("v1") // 创建一个v1组
	v1.GET("/", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "<h1>Hello my_gin</h1>")
	})
	v1.GET("/hello", func(c *my_gin.Context) {
		c.String(http.StatusOK, "hello %s, you're at %s\n", c.Query("name"), c.Path)
	})

	v2 := r.Group("/v2") //创建一个v2组
	{
		v2.GET("/hello/:name", func(c *my_gin.Context) {
			// 比如 /hello/my_gin ，这里Q
			c.String(http.StatusOK, "hello %s, you'ar at %s\n", c.Query("name"), c.Path)
		})
		v2.POST("/login", func(c *my_gin.Context) {
			c.JSON(http.StatusOK, my_gin.H{
				"username": c.PostForm("username"),
				"password": c.PostForm("password"),
			})
		})
	}

	r.Run(":8080")
}
