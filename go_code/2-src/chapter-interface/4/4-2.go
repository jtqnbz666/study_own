// 4-2 map基本操作
package main

import "fmt"

func create() {
	fmt.Println("map创建方式:")
	// 1 字面值
	{
		m1 := map[string]string{
			"m1": "v1", // 定义时指定的初始key/value, 后面可以继续添加
		}
		_ = m1

	}

	// 2 使用make函数
	{
		m2 := make(map[string]string) // 创建时，里面不含元素，元素都需要后续添加
		m2["m2"] = "v2"               // 添加元素
		_ = m2

	}

	// 定义一个空的map
	{
		m3 := map[string]string{}
		m4 := make(map[string]string)
		_ = m3
		_ = m4
	}
}

func curd() {
	fmt.Println("map增删改查:")
	// 创建
	fmt.Println("建:")
	m := map[string]string{
		"a": "va",
		"b": "vb",
	}
	fmt.Println(len(m)) // len(m) 获得m中key/value对的个数

	// 增加，修改
	fmt.Println("增改:")
	{
		// k不存在为增加，k存在为修改
		m["c"] = ""
		m["c"] = "11"                      // 重复增加(key相同)，使用新的值覆盖
		fmt.Printf("%#v %#v\n", m, len(m)) // map[string]string{"a":"va", "b":"vb", "c":"11"} 3
	}

	// 查
	fmt.Println("查:")
	{
		// v := m[k] // 从m中取键k对应的值给v，如果k在m中不存在，则将value类型的零值赋值给v
		// v, ok := m[k] // 从m中取键k对应的值给v，如果k存在，ok=true,如果k不存在，将value类型的零值赋值给v同时ok=false
		// 查1 - 元素不存在
		v1 := m["x"] //
		v2, ok2 := m["x"]
		fmt.Printf("%#v %#v %#v\n", v1, v2, ok2) // "" "" false

		// 查2 - 元素存在
		v3 := m["a"]
		v4, ok4 := m["a"]
		fmt.Printf("%#v %#v %#v\n", v3, v4, ok4) //"va" "va" true
	}
	fmt.Println("删:")
	// 删， 使用内置函数删除k/v对
	{
		// delete(m, k) 将k以及k对应的v从m中删掉；如果k不在m中，不执行任何操作
		delete(m, "x")                     // 删除不存在的key,原m不影响
		delete(m, "a")                     // 删除存在的key
		fmt.Printf("%#v %#v\n", m, len(m)) // map[string]string{"b":"vb", "c":"11"} 2
		delete(m, "a")                     // 重复删除不报错,m无影响
		fmt.Printf("%#v %#v\n", m, len(m)) /// map[string]string{"b":"vb", "c":"11"} 2
	}
}

func travel() {
	fmt.Println("map遍历:")
	m := map[string]int{
		"a": 1,
		"b": 2,
	}
	for k, v := range m {
		fmt.Printf("k:[%v].v:[%v]\n", k, v) // 输出k,v值
	}

	var fruits = map[string]int{
		"apple":  2,
		"banana": 5,
		"orange": 8,
	}

	for name, score := range fruits {
		fmt.Println(name, score)
	}

	for name := range fruits {
		fmt.Println(name)
	}
}

func main() {
	create()
	curd()
	travel()
}
