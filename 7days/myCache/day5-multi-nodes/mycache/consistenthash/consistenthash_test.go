package consistenthash

import (
	"strconv"
	"testing"
)

func TestHashing(t *testing.T) {
	hash := New(3, func(key []byte) uint32 {
		i, _ := strconv.Atoi(string(key))
		return uint32(i)
	})

	hash.Add("6", "4", "2")
	//对应 02/12/22 04/14/24 /06/16/26

	testCases := map[string]string{
		//在没有加入机器8之前，这四个数字会访问到的机器对应的就是 2/2/4/2
		"2":  "2",
		"11": "2",
		"23": "4",
		"27": "2",
	}
	for k, v := range testCases {
		if hash.Get(k) != v {
			t.Errorf("Asking for %s, should have yielded %s", k, v)
		}
	}
	//增加了8, 18, 28 三个节点，因为有虚拟节点
	hash.Add("8")

	//27应该被映射到8上
	for k, v := range testCases {
		if hash.Get(k) != v {
			t.Errorf("Asking for %s, should have yielded %s", k, v)
		}
	}

}
