package main

import (
	"fmt"
	_ "runtime"
	"runtime/debug"
	"sync"
	"sync/atomic"
)

func main() {
	// 禁用GC，并保证在main函数执行结束前恢复GC
	defer debug.SetGCPercent(debug.SetGCPercent(-1))
	var count int32

	// 实现一个函数 ，生成新对象
	newFunc := func() interface{} {
		fmt.Println("newFunc:", count)
		return atomic.AddInt32(&count, 1)
	}
	pool := sync.Pool{New: newFunc}	// 传入生成对象的函数....

	// New 字段值的作用
	v1 := pool.Get()	// 调用GET接口去取
	fmt.Printf("v1: %v\n", v1)
	pool.Put(v1)		// 放回去
	// 临时对象池的存取
	pool.Put(newFunc())
	// pool.Put(newFunc())
	// pool.Put(newFunc())
	v2 := pool.Get()
	// pool.Put(v2)
	fmt.Printf("v2: %v\n", v2)		// 这个时候v1和v2应该是一样

	// 垃圾回收对临时对象池的影响
	// debug.SetGCPercent(100)
	// runtime.GC()
	v3 := pool.Get()
	fmt.Printf("v3: %v\n", v3)
	pool.New = nil
	v4 := pool.Get()
	fmt.Printf("v4: %v\n", v4)
}
