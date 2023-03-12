package my_gin

import (
	"html/template"
	"net/http"
	"path"
	"strings"
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

		//html的模板
		htmlTemplates *template.Template
		funcMap       template.FuncMap
	}
)

func New() *Engine {
	engine := &Engine{router: NewRouter()}
	//注意最初始的匿名Group对象只初始化了engine, 说明其他都是空
	engine.RouterGroup = &RouterGroup{engine: engine}
	engine.groups = []*RouterGroup{engine.RouterGroup}
	return engine
}

//这种创建方式提供了中间件方法， 如日志和一场捕获
func Default() *Engine {
	engine := New()
	engine.Use(Logger(), Recovery())
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

func (group *RouterGroup) Use(middlewares ...HandlerFunc) {
	group.middlewares = append(group.middlewares, middlewares...)
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

//为html模板引入的方法

func (group *RouterGroup) createStaticHandler(relativePath string, fs http.FileSystem) HandlerFunc {
	absolutePath := path.Join(group.prefix, relativePath)
	//绝对路径 = 前缀 + 相对路径
	fileServer := http.StripPrefix(absolutePath, http.FileServer(fs)) // 返回的是一个接口
	return func(c *Context) {
		file := c.Param("filepath")
		//判断这个文件是否存在，以及我们是否有权限去访问它
		if _, err := fs.Open(file); err != nil {
			c.Status(http.StatusNotFound)
			return
		}
		fileServer.ServeHTTP(c.Writer, c.Req)
	}
}

//静态的文件
func (group *RouterGroup) Static(relativePath string, root string) {
	handler := group.createStaticHandler(relativePath, http.Dir(root))
	urlPattern := path.Join(relativePath, "/*filepath")
	//注册GET方法处理
	group.GET(urlPattern, handler)
}

//自定义渲染功能
func (engine *Engine) SetFuncMap(funcMap template.FuncMap) {
	engine.funcMap = funcMap
}

func (engine *Engine) LoadHTMLGlob(pattern string) {
	engine.htmlTemplates = template.Must(template.New("").Funcs(engine.funcMap).ParseGlob(pattern))
}

func (e *Engine) Run(addr string) (err error) {
	return http.ListenAndServe(addr, e)
}

//这里是突破口，理思路
func (e *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	var middlewares []HandlerFunc
	for _, group := range e.groups {
		//只要具有相同前缀的组， 都给它们加上这个处理函数
		if strings.HasPrefix(req.URL.Path, group.prefix) {
			//用group.prefix匹配req.URL.Path的前缀
			middlewares = append(middlewares, group.middlewares...)
		}
	}
	c := NewContext(w, req)
	c.handlers = middlewares
	c.engine = e
	e.router.handle(c)
}
