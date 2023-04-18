// 2.6 接口的组合继承
package main

import "fmt"

// 可以闻
type Smellable interface {
	smell()
}

// 可以吃
type Eatable interface {
	eat()
}

type Fruitable interface {
	Smellable
	Eatable
}

// 苹果既可能闻又能吃
type Apple struct{}

func (a Apple) smell() {
	fmt.Println("apple can smell")
}

func (a Apple) eat() {
	fmt.Println("apple can eat")
}

// 花只可以闻
type Flower struct{}

func (f Flower) smell() {
	fmt.Println("flower can smell")
}

// func TestType(items ...interface{}) {
// 	for k, v := range items {
// 		switch v.(type) {
// 		case string:
// 			fmt.Printf("type is string, %d[%v]\n", k, v)
// 		case bool:
// 			fmt.Printf("type is bool, %d[%v]\n", k, v)
// 		case int:
// 			fmt.Printf("type is int, %d[%v]\n", k, v)
// 		case float32, float64:
// 			fmt.Printf("type is float, %d[%v]\n", k, v)
// 		case Smellable:
// 			fmt.Printf("type is Smellable, %d[%v]\n", k, v)
// 		case *Smellable:
// 			fmt.Printf("type is *Smellable, %d[%p]\n", k, v)
// 		case Eatable:
// 			fmt.Printf("type is Eatable, %d[%v]\n", k, v)
// 		case *Eatable:
// 			fmt.Printf("type is Eatable, %d[%p]\n", k, v)
// 		case Fruitable:
// 			fmt.Printf("type is Fruitable, %d[%v]\n", k, v)
// 		case *Fruitable:
// 			fmt.Printf("type is Fruitable, %d[%p]\n", k, v)
// 		}
// 	}
// }
func main() {
	var s1 Smellable
	var s2 Eatable
	var apple = Apple{}
	var flower = Flower{}
	s1 = apple
	s1.smell()
	s1 = flower
	s1.smell()
	s2 = apple
	s2.eat()

	fmt.Println("\n组合继承")
	var s3 Fruitable
	s3 = apple
	s3.smell()
	s3.eat()

	// TestType(s1, s2, s3, apple, flower)
}
