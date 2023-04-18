package main

import (
	"fmt"
	"sync"
)

type SafeDict struct {
	data  map[string]int
	mutex *sync.Mutex
}

func NewSafeDict(data map[string]int) *SafeDict {
	return &SafeDict{
		data:  data,
		mutex: &sync.Mutex{},
	}
}

// defer 语句总是要推迟到函数尾部运行，所以如果函数逻辑运行时间比较长，
// 这会导致锁持有的时间较长，这时使用 defer 语句来释放锁未必是一个好注意。
func (d *SafeDict) Len() int {
	d.mutex.Lock()
	defer d.mutex.Unlock()
	return len(d.data)
}

// func (d *SafeDict) Test() int {
// 	d.mutex.Lock()
// 	length := len(d.data)
// 	d.mutex.Unlock() // 手动解锁 减少粒度	// 这种情况就不要用 defer d.mutex.Unlock()
// 	fmt.Println("length: ", length)
// 	// 这里还有耗时处理 耗时1000ms
// }

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
		delete(d.data, key)
	}
	return old_value, ok
}

func write(d *SafeDict) {
	d.Put("banana", 5)
}

func read(d *SafeDict) {
	fmt.Println(d.Get("banana"))
}

// go run -race 3-2-lock.go
func main() {
	d := NewSafeDict(map[string]int{
		"apple": 2,
		"pear":  3,
	})
	go read(d)
	write(d)
}
