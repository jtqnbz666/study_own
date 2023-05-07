//负责与外部交互，控制缓存存储和获取得主流程
package mycache

import (
	"fmt"
	"log"
	"sync"
)

type Getter interface {
	Get(key string) ([]byte, error)
}

// 就像 type MyInt int 一样的奇怪的使用方式
//如果不提供这个把函数转换为接口的函数，你调用时就需要创建一个struct，
//然后实现对应的接口，创建一个实例作为参数，相比这种方式就麻烦得多了。
type GetterFunc func(key string) ([]byte, error)

//接口型函数
func (f GetterFunc) Get(key string) ([]byte, error) {
	return f(key)
}

//一个group可以认为是一个缓存的命名空间，每个group拥有一个唯一的名称
//比如创建三个group，缓存学生的成绩命名为scores,缓存学生信息命名为info，缓存学生课程命名为courses
type Group struct {
	name      string
	getter    Getter //接口型函数,未命中时获取源数据的回调
	mainCache cache  //并发缓存的保证，加锁
	peers     PeerPicker
}

var (
	mu     sync.RWMutex
	groups = make(map[string]*Group)
)

func NewGroup(name string, cacheBytes int64, getter Getter) *Group {
	if getter == nil {
		panic("nil Getter")
	}
	mu.Lock()
	defer mu.Unlock()
	g := &Group{
		name:      name,
		getter:    getter,
		mainCache: cache{cacheBytes: cacheBytes},
	}
	groups[name] = g
	return g
}

func GetGroup(name string) *Group {
	mu.RLock()
	g := groups[name]
	mu.RUnlock()
	return g
}

func (g *Group) Get(key string) (ByteView, error) {
	if key == "" {
		return ByteView{}, fmt.Errorf("key is required")
	}
	if v, ok := g.mainCache.get(key); ok {
		log.Println("[GeeCache] hit")
		return v, nil //如果找到了直接返回就是了
	}
	return g.load(key)
}

func (g *Group) load(key string) (value ByteView, err error) {
	return g.getLocally(key)
}

func (g *Group) getLocally(key string) (ByteView, error) {
	bytes, err := g.getter.Get(key) //这是调用了未命中情况的处理函数
	if err != nil {
		return ByteView{}, err
	}
	value := ByteView{b: cloneBytes(bytes)}
	g.populateCache(key, value) //填充的意思， 加入缓存中
	return value, nil
}

func (g *Group) populateCache(key string, value ByteView) {
	g.mainCache.add(key, value)
}
