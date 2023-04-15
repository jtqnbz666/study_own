package my_gin

import (
	"net/http"
)

type router struct {
	handler map[string]HandlerFunc
}

func NewRouter() *router {
	return &router{handler: make(map[string]HandlerFunc)}
}

func (r *router) AddRouter(method string, pattern string, handler HandlerFunc) {
	key := method + "-" + pattern
	r.handler[key] = handler
}

func (r *router) handle(c *Context) {
	key := c.Method + "-" + c.Path
	if handler, ok := r.handler[key]; ok {
		handler(c)
	} else {
		c.String(http.StatusNotFound, "404 NOT FOUND:%s\n", c.Path)
	}
}
