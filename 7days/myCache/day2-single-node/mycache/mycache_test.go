package mycache

import (
	"fmt"
	"log"
	"testing"
)

var db = map[string]string{
	"jt": "666",
	"wh": "667",
	"lx": "668",
}

func TestGet(t *testing.T) {
	loadCounts := make(map[string]int, len(db))
	mytest := NewGroup("scores", 2<<10, GetterFunc(
		func(key string) ([]byte, error) {
			log.Println("[SlowDB] search key", key)
			if v, ok := db[key]; ok {
				if _, ok := loadCounts[key]; !ok {
					loadCounts[key] = 0
				}
				loadCounts[key] += 1
				return []byte(v), nil
			}
			return nil, fmt.Errorf("%s not exist", key)
		}))

	for k, v := range db {

		//第一次访问是不存在缓存的，但会加入缓存中
		if view, err := mytest.Get(k); err != nil || view.String() != v {
			t.Fatal("failed to get value of jt")
		}

		//这是第二次访问，就会命中缓存
		//如果loadCounts[key] > 1 说明触发了回调函数多次，没有缓存
		if _, err := mytest.Get(k); err != nil || loadCounts[k] > 1 {
			t.Fatalf("cache %s miss", k)
		}
	}
	if view, err := mytest.Get("unknown"); err == nil {
		t.Fatalf("the value of unknow should be empty, but %s got", view)
	}
}
