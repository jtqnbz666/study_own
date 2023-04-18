package main

import (
	"fmt"
	"sync"
	"time"
)

type test struct {
	name string
	age  int
}

type SafeDict struct {
	data map[string]int
	*sync.Mutex
	*test //指针对象，那么初始化的时候就必须用 &
	// tt *sync.Mutex  若采用有名对象 和21行对应
}

func NewSafeDict(data map[string]int) *SafeDict {
	return &SafeDict{
		data,
		&sync.Mutex{},
		&test{"jt", 19}, //若是这种按照顺序初始化，必须初始化每个对象
		//这里采用了顺序初始化， 所以可以不用写自己的成员对象

		//若采用有名对象
		// data : data,
		// tt: &sync.Mutex{},

		//这样是错误的， 普通结构体test适用，但sync.Mutex的匿名对象不管是否指针，目前只发现顺序初始化一种方式
		// data:       data,
		// test:       &test{},
		// sync.Mutex: sync.Mutex{},
	}
}

// func (d *SafeDict) Len() int {
// 	d.Lock() //采用匿名对象的好处就是可以直接这样使用
// 	defer d.Unlock()
// 	return len(d.data)
// }

// func (d *SafeDict) Put(key string, value int) (int, bool) {
// 	d.Lock()
// 	defer d.Unlock()
// 	old_value, ok := d.data[key]
// 	d.data[key] = value
// 	return old_value, ok
// }

// func (d *SafeDict) Get(key string) (int, bool) {
// 	d.Lock()
// 	defer d.Unlock()
// 	old_value, ok := d.data[key]
// 	return old_value, ok
// }

// func (d *SafeDict) Delete(key string) (int, bool) {
// 	d.Lock()
// 	defer d.Unlock()
// 	old_value, ok := d.data[key]
// 	if ok {
// 		delete(d.data, key)
// 	}
// 	return old_value, ok
// }

// func write(d *SafeDict) {
// 	d.Put("banana", 6)
// }

// func read(d *SafeDict) {
// 	fmt.Println(d.Get("banana"))
// }

func main() {
	d := NewSafeDict(map[string]int{
		"apple": 2,
		"pear":  3,
	})
	//go read(d)
	//write(d)
	time.Sleep(3 * time.Second)
	fmt.Println(d)
}
