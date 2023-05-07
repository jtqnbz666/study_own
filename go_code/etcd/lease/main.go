package main

import (
	"context"
	"fmt"

	"github.com/coreos/etcd/clientv3"
)

func main() {
	cli, err := clientv3.NewFromURL("127.0.0.1:2379")
	if err != nil {
		panic(err)
	}
	defer cli.Close()
	lease, err := cli.Grant(context.Background(), 5)
	if err != nil {
		panic(err)
	}
	fmt.Println("lease id", lease.ID)
	_, err = cli.Put(context.Background(), "key", "mark", clientv3.WithLease(lease.ID))
	if err != nil {
		panic(err)
	}

	if false {
		// 续租 长期续租  短期续租
		ch, err := cli.KeepAlive(context.Background(), lease.ID)
		if err != nil {
			panic(err)
		}
		for {
			recv := <-ch
			fmt.Println("time to live", recv.TTL)
		}
	}
	if true {
		res, err := cli.KeepAliveOnce(context.Background(), lease.ID)
		if err != nil {
			panic(err)
		}
		fmt.Println("time to live", res.TTL)
	}
}
