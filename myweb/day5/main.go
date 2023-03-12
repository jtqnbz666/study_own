package main

import (
	"log"
	"my_gin"
	"net/http"
	"time"
)

func OnlyForV2() my_gin.HandlerFunc {
	return func(c *my_gin.Context) {
		//开始时间
		t := time.Now()
		//如果发生了一个错误
		c.Fail(500, "Internale Server error")
		log.Printf("[%d] %s in %v for group v2", c.StatusCode, c.Req.RequestURI, time.Since(t))
	}
}

func main() {
	r := my_gin.New()
	r.Use(my_gin.Logger()) //全局的中间件
	r.GET("/", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "<h1>Hello jt</h1>")
	})

	v2 := r.Group("/v2")
	v2.Use(OnlyForV2())
	{
		v2.GET("/hello/:name", func(c *my_gin.Context) {
			c.String(http.StatusOK, "hello %s, you are at %s\n", c.Param("name"), c.Path)
		})
	}
	r.Run(":8080")
}
