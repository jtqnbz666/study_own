package main

import (
	"fmt"
	// "time"

	"github.com/gomodule/redigo/redis"
)

func main() {
	c, err := redis.Dial("tcp", "192.168.2.132:6379")
	c, err := redis.Dial("tcp", "192.168.2.132:6379", 
		redis.DialConnectTimeout(time.Duration(1) * time.Second),
		redis.DialPassword("111"),
		redis.DialDatabase(1))
	if err != nil {
		fmt.Println("conn redis failed,", err)
		return
	}

	defer c.Close()
}
