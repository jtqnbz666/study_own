//缓存值得抽象与封装

package mycache

type ByteView struct {
	b []byte //存储真实的缓存值
	//用byte是为了支持任意格式的存储，比如字符串和图片
}

func (v ByteView) Len() int {
	return len(v.b)
}

func (v ByteView) ByteSlice() []byte {
	return cloneBytes(v.b)
}

func (v ByteView) String() string {
	return string(v.b)
}

func cloneBytes(b []byte) []byte {
	c := make([]byte, len(b))
	copy(c, b)
	return c
}
