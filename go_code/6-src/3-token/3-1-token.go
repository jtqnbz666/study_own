package main

import (
	"bytes"
	"crypto/rand"
	"crypto/sha1"
	"fmt"
	"math/big"
	"sort"
	"strconv"
	"time"
)

func CreateRandomString(len int) string {
	var container string
	var str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
	b := bytes.NewBufferString(str)
	length := b.Len()
	bigInt := big.NewInt(int64(length))
	for i := 0; i < len; i++ {
		randomInt, _ := rand.Int(rand.Reader, bigInt)
		container += string(str[randomInt.Int64()])
	}
	return container
}

// 根据 Token Timestamp Nonce 生成对应的校验码， Token是不能明文传输的
func GenerateSignature(token string) (timestamp string, nonce string, signature string) {

	nonce = CreateRandomString(10)
	timestamp = strconv.FormatInt(time.Now().Unix(), 10) //int64转字符串
	// 排序 微信约定好的
	strs := sort.StringSlice{token, timestamp, nonce} // 使用本地的token生成校验
	sort.Strings(strs)                                // strs: [1607173019 liaoqingfu qvCyrKEuoS]
	fmt.Println("strs:", strs)                        // 排序
	str := ""
	for _, s := range strs {
		str += s // 拼接字符串
	}
	fmt.Println("str:", str) //str: 1607173019liaoqingfuqvCyrKEuoS
	h := sha1.New()		// 完全都是自己的服务的时候 你这里你用md5
	h.Write([]byte(str))                      // 转成byte
	signature = fmt.Sprintf("%x", h.Sum(nil)) // h.Sum(nil) 做hash  79efadd80a344c0b73b3bd2c403184f7425a5a67
	return
}

func VerifySignature(token string, timestamp string, nonce string, signature string) bool {
	// str = token + timestamp + nonce
	strs := sort.StringSlice{token, timestamp, nonce} // 使用本地的token生成校验
	sort.Strings(strs)
	str := ""
	for _, s := range strs {
		str += s
	}
	h := sha1.New()	// 完全都是自己的服务的时候 你这里你用md5
	h.Write([]byte(str))
	return fmt.Sprintf("%x", h.Sum(nil)) == signature
}
func main() {
	token := "liaoqingfu"
	// 产生签名
	timestamp, nonce, signature := GenerateSignature(token) // 发送服务器的时候是发送  timestamp, nonce, signature
	fmt.Printf("1. token %s -> 产生签名:%s, timestamp:%s, nonce:%s\n", token, signature, timestamp, nonce)
	// 验证签名
	ok := VerifySignature(token, timestamp, nonce, signature) // 服务进行校验
	if ok {
		fmt.Println("2. 验证签名正常")
	} else {
		fmt.Println("2. 验证签名失败")
	}
}
