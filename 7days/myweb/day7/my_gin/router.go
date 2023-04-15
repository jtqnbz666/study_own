package my_gin

import (
	"net/http"
	"strings"
)

type router struct {
	handlers map[string]HandlerFunc // handlers和之前一样的作用
	roots    map[string]*node       // 拆分路径并存储
}

func NewRouter() *router {
	return &router{
		handlers: make(map[string]HandlerFunc),
		roots:    make(map[string]*node),
	}
}

// 解析完整路径，拆分为一个一个的单词
func parsePattern(pattern string) []string {
	vs := strings.Split(pattern, "/")

	/* 打印结果
	0
	1 v1
	2 zoo
	3 xm
	*/

	parts := make([]string, 0)
	for _, item := range vs {
		if item != "" { //空的不会放进去
			parts = append(parts, item)
			if item[0] == '*' { //允许存在一个 "*"
				break
			}
		}
	}
	return parts //是不包含空字符的
}

// 添加路由规则
func (r *router) AddRoute(method string, pattern string, handler HandlerFunc) {
	parts := parsePattern(pattern) //解析出来后，parts数组是不包含空字符的
	key := method + "-" + pattern
	_, ok := r.roots[method]
	if !ok {
		r.roots[method] = &node{} // root是一个 map[string]*node 类型
	}

	//构造路径树
	r.roots[method].insert(pattern, parts, 0)

	// 还是和之前一样的方式，直接建立映射
	r.handlers[key] = handler

}

//为什么能通过':' , '*' 获取具体路径，就是在这里处理的
func (r *router) getRoute(method string, path string) (*node, map[string]string) {
	searchParts := parsePattern(path) //把一个完整路径拆分为一个一个的单词
	params := make(map[string]string)
	root, ok := r.roots[method] // ok会返回成功与否
	// 比如method是GET, 如果不ok ，说明还没有注册过GET方法的处理函数
	if !ok {
		return nil, nil
	}

	n := root.search(searchParts, 0) //找到最深层级，如[v1, zoo, xm]中找到xm对应的node
	if n != nil {
		parts := parsePattern(n.pattern)
		for index, part := range parts {
			if part[0] == ':' {
				params[part[1:]] = searchParts[index]
			}
			if part[0] == '*' && len(part) > 1 {
				params[part[1:]] = strings.Join(searchParts[index:], "/")
				//strings.Join 的作用是给searchParts数组中的每一个元素拼加上"/"
				break
			}
		}
		return n, params
	}
	return nil, nil
}

//获取所有node节点
func (r *router) getRoutes(method string) []*node {
	root, ok := r.roots[method] //root 是 *node类型的
	if !ok {
		return nil
	}
	nodes := make([]*node, 0)
	root.travel(&nodes)
	return nodes
}

//突破口是这里， 从这里理思路
func (r *router) handle(c *Context) {
	n, params := r.getRoute(c.Method, c.Path)
	if n != nil {
		c.Params = params
		key := c.Method + "-" + n.pattern
		//怪不得ServHTTP的处理总是放到最后， 因为在最后才加入的它
		c.handlers = append(c.handlers, r.handlers[key])
	} else {
		c.handlers = append(c.handlers, func(c *Context) {
			c.String(http.StatusNotFound, "404 NOT FOUND: %s\n", c.Path)
		})
	}
	c.Next()
}
