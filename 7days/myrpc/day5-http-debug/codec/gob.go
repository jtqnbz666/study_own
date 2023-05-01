package codec

import (
	"bufio"
	"encoding/gob"
	"io"
	"log"
)

type GobCodec struct {
	conn io.ReadWriteCloser
	buf  *bufio.Writer
	dec  *gob.Decoder
	enc  *gob.Encoder
}

var _ Codec = (*GobCodec)(nil)

func NewGobCodec(conn io.ReadWriteCloser) Codec {
	buf := bufio.NewWriter(conn)
	return &GobCodec{
		conn: conn,
		buf:  buf,
		dec:  gob.NewDecoder(conn),
		enc:  gob.NewEncoder(buf),
	}
}

func (c *GobCodec) ReadHeader(h *Header) error {
	return c.dec.Decode(h)
}

func (c *GobCodec) ReadBody(body interface{}) error {
	return c.dec.Decode(body)
}

func (c *GobCodec) Write(h *Header, body interface{}) (err error) {
	defer func() {
		_ = c.buf.Flush() // 把写入的内容刷走，也就是发出去的意思
		if err != nil {
			_ = c.Close()
		}
	}()
	// 可以看到在初始化的时候是用的buf 来进行初始化的， 所以是写到了buf上做一个缓存， 当flush的时候才会刷到conn。
	if err = c.enc.Encode(h); err != nil {
		log.Println("rpc: 编码头部的时候失败")
		return
	}
	if err = c.enc.Encode(body); err != nil {
		log.Println("rpc: 编码身体的时候失败")
		return
	}
	return
}

func (c *GobCodec) Close() error {
	return c.conn.Close()
}
