package mycache

import (
	"fmt"
	"io/ioutil"
	"log"
	"mycache/consistenthash"
	"net/http"
	"net/url"
	"strings"
	"sync"
)

type HTTPPool struct {
	self        string //记录自己的地址(主机名/ip/port)
	basePath    string //作为节点间通讯的前缀
	mu          sync.Mutex
	peers       *consistenthash.Map    //用来根据key选择节点
	httpGetters map[string]*httpGetter //因为不同远程节点的baseURL不同，所以对应不同的这个成员，
}

type httpGetter struct {
	baseURL string //表示将要访问的远程节点的地址
}

const (
	defaultBasePath = "/_mycache"
	defaultReplicas = 50
)

func (h *httpGetter) Get(group string, key string) ([]byte, error) {
	u := fmt.Sprintf(
		"%v%v/%v",
		h.baseURL,
		//转义字符串，以便安全的将字符串放入url中
		url.QueryEscape(group),
		url.QueryEscape(key),
	)
	res, err := http.Get(u)
	if err != nil {
		return nil, err
	}
	defer res.Body.Close()
	if res.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("server returned: %v", res.Status)
	}

	bytes, err := ioutil.ReadAll(res.Body)
	if err != nil {
		return nil, fmt.Errorf("reading response body: %v", err)
	}
	return bytes, nil
}

var _ PeerGetter = (*httpGetter)(nil)

func NewHTTPPool(self string) *HTTPPool {
	return &HTTPPool{
		self:     self,
		basePath: defaultBasePath,
	}
}

//实例化一致性hash
//将远程节点的地址信息配置好
func (p *HTTPPool) Set(peers ...string) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.peers = consistenthash.New(defaultReplicas, nil)
	p.peers.Add(peers...)
	p.httpGetters = make(map[string]*httpGetter, len(peers))
	for _, peer := range peers {
		p.httpGetters[peer] = &httpGetter{baseURL: peer + p.basePath}
	}
}

//包装了一致性哈希算法中的Get()方法，即根据具体的key选择节点并返回节点给http客户端
func (p *HTTPPool) PickPeer(key string) (PeerGetter, bool) {
	p.mu.Lock()
	defer p.mu.Unlock()
	if peer := p.peers.Get(key); peer != "" && peer != p.self {
		p.Log("Pick peer %s", peer)

		//返回这个节点上的httpGetter方法，不同的节点有不同的httpGetter
		return p.httpGetters[peer], true
	}
	return nil, false
}

var _ PeerPicker = (*HTTPPool)(nil)

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
