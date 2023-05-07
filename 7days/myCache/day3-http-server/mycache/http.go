package mycache

import (
	"fmt"
	"log"
	"net/http"
	"strings"
)

const defaultBasePath = "/_mycache/"

type HTTPPool struct {
	self     string //记录自己的地址(主机名/ip/port)
	basePath string //作为节点间通讯的前缀
}

func NewHTTPPool(self string) *HTTPPool {
	return &HTTPPool{
		self:     self,
		basePath: defaultBasePath,
	}
}

func (p *HTTPPool) Log(format string, v ...interface{}) {
	log.Printf("[Server %s]%s", p.self, fmt.Sprintf(format, v...))
}

func (p *HTTPPool) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	//判断访问路径的前缀是否是basepath，不是则返回
	if !strings.HasPrefix(r.URL.Path, p.basePath) {
		panic("HTTPPool serving unexpected path:" + r.URL.Path)
	}
	p.Log("%s %s", r.Method, r.URL.Path)

	//路径格式为 /basepath/groupname/key
	//通过第二个值得到group的实例，再使用group.Get(key)得到缓存的数据
	parts := strings.SplitN(r.URL.Path[len(p.basePath):], "/", 2)
	if len(parts) != 2 {
		http.Error(w, "bad request", http.StatusBadRequest)
		return
	}

	groupName := parts[0]
	key := parts[1]

	group := GetGroup(groupName)
	if group == nil {
		http.Error(w, "no such group:"+groupName, http.StatusNotFound)
		return
	}

	view, err := group.Get(key)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	w.Header().Set("Content-Type", "application/octet-stream")
	w.Write(view.ByteSlice())
}
