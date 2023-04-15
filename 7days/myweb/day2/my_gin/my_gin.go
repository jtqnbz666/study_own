package my_gin

import (
	"log"
	"net/http"
)

type Engine struct {
	route *router
}

type HandlerFunc func(c *Context)

func New() *Engine {
	return &Engine{route: NewRouter()}
}

func (engine *Engine) AddRouter(method string, pattern string, handler HandlerFunc) {
	engine.route.AddRouter(method, pattern, handler)
}

func (engine *Engine) GET(pattern string, handler HandlerFunc) {
	engine.AddRouter("GET", pattern, handler)
}

func (engine *Engine) POST(pattern string, handler HandlerFunc) {
	engine.AddRouter("POST", pattern, handler)
}

func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	ctx := newContext(w, req)
	engine.route.handle(ctx)
}

func (engine *Engine) Run(addr string) {
	log.Fatal(http.ListenAndServe(addr, engine))
}
