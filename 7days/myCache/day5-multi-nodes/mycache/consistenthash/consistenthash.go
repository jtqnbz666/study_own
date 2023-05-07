package consistenthash

import (
	"hash/crc32"
	"sort"
	"strconv"
)

type Hash func(data []byte) uint32

type Map struct {
	hash     Hash
	replicas int            //虚拟节点的倍数
	keys     []int          //哈希环
	hashMap  map[int]string //虚拟节点与真实节点的映射表，健是虚拟节点的hash值，值是真实节点的名称
}

//允许自定义虚拟节点倍数和hash函数
func New(replicas int, fn Hash) *Map {
	m := &Map{
		replicas: replicas,
		hash:     fn,
		hashMap:  make(map[int]string),
	}
	if m.hash == nil {
		m.hash = crc32.ChecksumIEEE
	}
	return m
}

//添加真实节点的方法
func (m *Map) Add(keys ...string) {
	for _, key := range keys {
		for i := 0; i < m.replicas; i++ {
			hash := int(m.hash([]byte(strconv.Itoa(i) + key)))
			m.keys = append(m.keys, hash)
			m.hashMap[hash] = key
		}
	}
	sort.Ints(m.keys)
}

//选择节点的方法
func (m *Map) Get(key string) string {
	if len(m.keys) == 0 {
		return ""
	}

	hash := int(m.hash([]byte(key)))
	//顺时针找到第一个匹配的虚拟节点的下标idx，m.keys是排好序的，顺序找就行了
	idx := sort.Search(len(m.keys), func(i int) bool {
		return m.keys[i] >= hash
	})

	//如果idx == len(m.keys)说明应该选择m.keys[0],所以需要取余
	return m.hashMap[m.keys[idx%len(m.keys)]]
}
