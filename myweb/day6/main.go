package main

import (
	"fmt"
	"html/template"
	"my_gin"
	"net/http"
	"time"
)

type student struct {
	Name string
	Age  int
}

func FormatAsData(t time.Time) string {
	year, month, day := t.Date()
	return fmt.Sprintf("%d-%02d-%02d", year, month, day)
}

func main() {
	r := my_gin.New()
	r.Use(my_gin.Logger())
	r.SetFuncMap(template.FuncMap{
		"FormatAsDate": FormatAsData,
	})
	r.LoadHTMLGlob("templates/*")
	r.Static("/assets", "./static")

	stu1 := &student{Name: "jt", Age: 19}
	stu2 := &student{Name: "wh", Age: 18}
	r.GET("/", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "css.tmpl", nil) // 以前这里都是直接<h1>hello,jt</h1>,现在改进了
	})
	r.GET("/students", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "arr.tmpl", my_gin.H{
			"title":  "my_gin",
			"stuArr": [2]*student{stu1, stu2},
		})
	})
	r.GET("/date", func(c *my_gin.Context) {
		c.HTML(http.StatusOK, "custom_func.tmpl", my_gin.H{
			"title": "my_gin",
			"now":   time.Date(2023, 3, 11, 11, 39, 0, 0, time.UTC),
		})
	})
	r.Run(":8080")
}
