package my_gin

import (
	"log"
	"time"
)

func Logger() HandlerFunc {
	return func(c *Context) {
		//开启时间
		t := time.Now()
		//处理进行请求
		c.Next()
		//计算处理时间
		log.Printf("[%d] %s in %v", c.StatusCode, c.Req.RequestURI, time.Since(t))
	}
}
