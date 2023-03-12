package my_gin

import (
	"fmt"
	"log"
	"net/http"
	"runtime"
	"strings"
)

func trace(message string) string {
	var pcs [32]uintptr
	//跳过前三个栈上的信息， 它们分别是当前函数，这个函数的调用者，以及再上一个调用者
	//对应到当前也就是 main.main,  my_gin.default, my_gin.Use
	n := runtime.Callers(3, pcs[:])

	var str strings.Builder
	str.WriteString(message + "\nTraceback:")
	for _, pc := range pcs[:n] {
		fn := runtime.FuncForPC(pc)
		file, line := fn.FileLine(pc)
		str.WriteString(fmt.Sprintf("\n\t%s:%d", file, line))
	}
	return str.String()
}

//func Caller(skip int) (pc uintptr, file string, line int, ok bool)
// pc是uintptr这个返回的是函数指针

// file是函数所在文件名目录

// line所在行号

// ok 是否可以获取到信息

func Recovery() HandlerFunc {
	return func(c *Context) {
		defer func() {
			if err := recover(); err != nil {
				message := fmt.Sprintf("%s", err)
				log.Printf("%s\n\n", trace(message))
				c.Fail(http.StatusInternalServerError, "Internal Server Error")
			}
		}()

		c.Next()
	}
}
