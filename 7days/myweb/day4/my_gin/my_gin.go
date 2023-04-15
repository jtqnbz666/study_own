package my_gin

import (
	"net/http"
)

type HandlerFunc func(*Context)
type (
	RouterGroup struct {
		prefix      string
		middlewares []HandlerFunc //支持中间件， 如认证等服务
		parent      *RouterGroup  //支持嵌套
		engine      *Engine       //所有组共享一个engine
	}

	Engine struct {
		router *router

		//类似于继承，包含另一个匿名结构体就相当于继承了该结构体的字段和方法。
		//可以直接访问它的对象和方法
		*RouterGroup
		groups []*RouterGroup //存储所有的group
	}
)

func New() *Engine {
	engine := &Engine{router: NewRouter()}
	engine.RouterGroup = &RouterGroup{engine: engine}
	engine.groups = []*RouterGroup{engine.RouterGroup}
	return engine
}

// 新增一个组
func (group *RouterGroup) Group(prefix string) *RouterGroup {
	engine := group.engine
	newGroup := &RouterGroup{
		prefix: group.prefix + prefix,
		parent: group,
		engine: engine, //共用同一个engin
	}
	engine.groups = append(engine.groups, newGroup)
	return newGroup
}

func (group *RouterGroup) AddRoute(method string, comp string, handler HandlerFunc) {
	pattern := group.prefix + comp
	group.engine.router.AddRoute(method, pattern, handler)
}

func (group *RouterGroup) GET(pattern string, handler HandlerFunc) {
	group.AddRoute("GET", pattern, handler)
}

func (group *RouterGroup) POST(pattern string, handler HandlerFunc) {
	group.AddRoute("POST", pattern, handler)
}

func (e *Engine) Run(addr string) (err error) {
	return http.ListenAndServe(addr, e)
}

//这里是突破口，理思路
func (e *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	c := NewContext(w, req)
	e.router.handler(c)
}
