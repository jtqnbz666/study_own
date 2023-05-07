package main

import (
	"context"
	"fmt"

	"github.com/coreos/etcd/clientv3"
	"github.com/coreos/etcd/clientv3/concurrency"
)

func main() {
	cli, err := clientv3.New(clientv3.Config{
		Endpoints: []string{"127.0.0.1:2379"},
	})
	if err != nil {
		panic(err)
	}
	defer cli.Close()
	s1, err := concurrency.NewSession(cli, concurrency.WithContext(context.Background()), concurrency.WithTTL(10))
	if err != nil {
		panic(err)
	}
	defer s1.Close()
	m1 := concurrency.NewMutex(s1, "lock")

	s2, err := concurrency.NewSession(cli, concurrency.WithContext(context.Background()))
	if err != nil {
		panic(err)
	}
	defer s2.Close()
	m2 := concurrency.NewMutex(s2, "lock")

	if err := m1.Lock(context.Background()); err != nil {
		panic(err)
	}
	fmt.Println("s1 acquired lock")

	m2ch := make(chan struct{})
	go func() {
		defer close(m2ch)
		if err := m2.Lock(context.Background()); err != nil {
			panic(err)
		}
	}()

	if err := m1.Unlock(context.Background()); err != nil {
		panic(err)
	}
	fmt.Println("s1 released lock")

	<-m2ch
	fmt.Println("s2 acquired lock")
}
