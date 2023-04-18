package main

import (
	"fmt"
	"sync"
	"time"
)

type SafeDict struct {
	data  map[string]int
	mutex *sync.Mutex
}

func NewSafeDict(data map[string]int) *SafeDict {
	return &SafeDict{
		data:  data,
		mutex: &sync.Mutex{}, //初始化了
	}
}

func (d *SafeDict) Len() int {
	d.mutex.Lock()
	defer d.mutex.Unlock()
	//如果后边有比较耗时的操作，就不要使用defer
	return len(d.data)
}

func (d *SafeDict) Put(key string, value int) (int, bool) {
	d.mutex.Lock()
	defer d.mutex.Unlock()
	old_value, ok := d.data[key]
	d.data[key] = value
	return old_value, ok
}

func (d *SafeDict) Get(key string) (int, bool) {
	d.mutex.Lock()
	defer d.mutex.Unlock()
	old_value, ok := d.data[key]
	return old_value, ok
}

func (d *SafeDict) Delete(key string) (int, bool) {
	d.mutex.Lock()
	defer d.mutex.Unlock()
	old_value, ok := d.data[key]
	if ok {
		delete(d.data, key) //删除字典中对应key的数据
	}
	return old_value, ok
}

func write(d *SafeDict) {
	d.Put("banana", 5)
}

func read(d *SafeDict) {
	fmt.Println(d.Get("banana"))
}

func main() {
	d := NewSafeDict(map[string]int{
		"apple": 2,
		"pear":  3,
	})
	go read(d)
	write(d)
	time.Sleep(time.Second)
}
