package my_gin

import (
	"net/http"
)

type HandlerFunc func(*Context)
type Engine struct {
	router *router
}

func New() *Engine {
	return &Engine{router: NewRouter()}
}

func (e *Engine) AddRoute(method string, pattern string, handler HandlerFunc) {
	e.router.AddRoute(method, pattern, handler)
}

func (e *Engine) GET(pattern string, handler HandlerFunc) {
	e.AddRoute("GET", pattern, handler)
}

func (e *Engine) POST(pattern string, handler HandlerFunc) {
	e.AddRoute("POST", pattern, handler)
}

func (e *Engine) Run(addr string) (err error) {
	return http.ListenAndServe(addr, e)
}

//这里是突破口，理思路
func (e *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	c := NewContext(w, req)
	e.router.handler(c)
}
