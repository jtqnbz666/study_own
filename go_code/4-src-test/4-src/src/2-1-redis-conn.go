package main

import(
	"fmt"
	"time"
	"github.com/gomodule/redigo/redis"
)

func main() {
	//c, err := redis.Dial("tcp", "127.0.0.1:6379")
	c, err := redis.Dial("tcp", "127.0.0.1:6379", 
		redis.DialConnectTimeout(time.Duration(1) * time.Second),
		redis.DialPassword(""),
		redis.DialDatabase(1))
	if err != nil {
		fmt.Println("conn redis failed,", err)
		return
	}

	defer c.Close()
}
