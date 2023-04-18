// 2.4 关闭通道
/*
Go 语言的通道有点像文件，不但支持读写操作， 还支持关闭。
读取一个已经关闭的通道会立即返回通道类型的「零值」，而写一个已经关闭的通道会抛异常。
如果通道里的元素是整型的，读操作是不能通过返回值来确定通道是否关闭的。
*/
package main

import "fmt"

func main() {
	var ch = make(chan int, 4)
	ch <- 1
	ch <- 2

	fmt.Println("len(ch):", len(ch), "cap(ch):", cap(ch))
	close(ch)

	value := <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	value = <-ch
	fmt.Println(value)
	ch <- 3
}
