package main

import (
	"net/http"

	"my_gin"
)

func main() {
	r := my_gin.New()
	r.GET("/", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "<h1>Hello jt</h1>")
	})

	r.GET("/hello", func(c *my_gin.Context) {
		c.String(http.StatusOK, "hello %s, you're at %s\n", c.Query("name"), c.Path)
	})

	
	r.GET("/hello/:name", func(c *my_gin.Context) {
		c.String(http.StatusOK, "hello %s, you're at %s\n", c.Param("name"), c.Path)
	})

	r.GET("/assets/*filepath", func(c *my_gin.Context) {
		c.JSON(http.StatusOK, my_gin.H{
			"filepath": c.Param("filepath"),
		})
	})

	r.Run(":8080")
}
