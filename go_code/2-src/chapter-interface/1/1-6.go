package main

import "fmt"

type Person struct {
	Name string
	Age  int
}

func (p Person) Getname() string { //p代表结构体本身的实列，类似python中的self,这里p可以写为self
	fmt.Println(p.Name)
	return p.Name
}

func (p Person) Setname(name string) {
	fmt.Printf("Setname:%p\n", &p)
	p.Name = name
}

func (p *Person) Setname2(name string) {
	fmt.Printf("Setname2:%p\n", p)
	p.Name = name
}

// go不会用->，不管是指针还是值 都是 .
func main() {
	var person1 = new(Person)
	fmt.Printf("person1:%p\n", person1)
	person1.Age = 34
	person1.Name = "darren"
	person1.Getname() // darren
	person1.Setname("king")
	person1.Getname() // darren
	person1.Setname2("柚子老师")
	person1.Getname() // 柚子老师
}
