package main

import (
	"fmt"
	"net"
	"net/http"
	"time"
)

func main() {

	url := "http://www.baidu1.com"
	c := http.Client{
		Transport: &http.Transport{
			Dial: func(network, addr string) (net.Conn, error) {
				timeout := time.Second * 2
				return net.DialTimeout(network, addr, timeout)
			},
		},
	}
	resp, err := c.Head(url)
	if err != nil {
		fmt.Printf("head %s failed, err:%v\n", url, err)
	} else {
		fmt.Printf("%s head succ, status:%v\n", url, resp.Status)
	}

}
