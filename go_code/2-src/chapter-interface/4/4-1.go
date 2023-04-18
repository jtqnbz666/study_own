package main

import "fmt"

func main() {
	// m0 可以, key类型为string, 支持 == 比较操作
	{
		fmt.Println("---- m0 ----")
		var m0 map[string]string // 定义map类型变量m0，key的类型为string，value的类型string
		fmt.Println(m0)
	}

	// m1 不可以, []byte是slice，不支持 == != 操作，不可以作为map key的数据类型
	{
		// fmt.Println("---- m1 ----");
		//var m1 map[[]byte]string // 报错： invalid map key type []byte
		//fmt.Println(m1)

		// 准确说slice类型只能与nil比较，其他的都不可以，可以通过如下测试：
		// var b1,b2 []byte
		// fmt.Println(b1==b2) // 报错： invalid operation: b1 == b2 (slice can only be compared to nil)
	}

	// m2 可以, interface{}类型可以作为key，但是需要加入的key的类型是可以比较的
	{
		fmt.Println("---- m2 ----")
		var m2 map[interface{}]string
		m2 = make(map[interface{}]string)
		//m2[[]byte("k2")]="v2" // panic: runtime error: hash of unhashable type []uint8
		m2[123] = "123"
		m2[12.3] = "123"
		fmt.Println(m2)

		var str string = m2[12.3]
		fmt.Println(str)
	}

	// m3 可以， 数组支持比较
	{
		fmt.Println("---- m3 ----")
		a3 := [3]int{1, 2, 3}
		var m3 map[[3]int]string
		m3 = make(map[[3]int]string)
		m3[a3] = "m3"
		fmt.Println(m3)
	}

	// m4 可以，book1里面的元素都是支持== !=
	{
		fmt.Println("---- m4 ----")
		type book1 struct {
			name string
		}
		var m4 map[book1]string
		fmt.Println(m4)
	}

	// m5 不可以, text元素类型为[]byte, 不满足key的要求
	{
		fmt.Println("---- m5 ----")
		// type book2 struct {
		// 	name string
		// 	text []byte //没有这个就可以
		// }
		//var m5 map[book2]string //invalid map key type book2
		//fmt.Println(m5)
	}
}
