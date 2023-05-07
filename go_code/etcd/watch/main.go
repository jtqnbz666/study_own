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

	watch := cli.Watch(context.Background(), "key3", clientv3.WithFromKey())
	for resp := range watch {
		for _, ev := range resp.Events {
			fmt.Printf("Type: %s Key: %s Value: %s\n", ev.Type, ev.Kv.Key, ev.Kv.Value)
		}
	}
}
