package my_gin

import (
	"fmt"
	"strings"
)

type node struct {
	pattern  string //完整路径 比如/v1/zoo/xm
	part     string //具体的某一段路径 v1 或 zoo 或 xm
	children []*node
	isWild   bool //是否是 "*" 或 ":"
}

func (n *node) String() string {
	return fmt.Sprintf("node{pattern=%s, part=%s, isWild=%t}", n.pattern, n.part, n.isWild)
}

//height表示 路径的第几层级， pattern表示完整路径，parts装着路径拆分后的段
//这个函数主要是相当于把路径树 构建出来
func (n *node) insert(pattern string, parts []string, height int) {
	//一般都是从height = 0传进来的
	if len(parts) == height {
		n.pattern = pattern
		return
	}
	//比如传入 part为[v1, zoo, xm] ,pattern为/v1/zoo/xm
	part := parts[height]

	//判断当前层级是否已经含有该层级 如当height=1时，就判断是否有[v1, zoo, xm]中的zoo层级
	child := n.matchChild(part) //先在n.children中遍历查找一遍
	if child == nil {
		child = &node{
			part:   part,
			isWild: part[0] == ':' || part[0] == '*',
		}
		n.children = append(n.children, child)
	}
	child.insert(pattern, parts, height+1)
}

//找到part 相同的那个node， part表示某一个具体的单词(即一条完整uri的某一个层级)
func (n *node) matchChild(part string) *node {
	for _, child := range n.children {
		if child.part == part || child.isWild {
			return child
		}
	}
	return nil
}

//比如uri为/v1/zoo/xiongmao，通过改变height值不断递归下去，直到找到part 为 xiongmao的node
//遍历路径树
func (n *node) search(parts []string, height int) *node { //height是从0 开始的
	if len(parts) == height || strings.HasPrefix(n.part, "*") {
		//strings.HasPrefix(n.part, "*") 检测n.part是否以"*"开头
		if n.pattern == "" {
			return nil
		}
		return n
	}

	part := parts[height]
	children := n.matchChildren(part) //可能含有多个节点

	for _, child := range children {
		// 一层一层递归下去，找到了就返回结果
		result := child.search(parts, height+1)
		if result != nil {
			return result
		}
	}
	return nil
}

// 返回符合当前层级的所有node， 比如/v1/hello, /v1/jt  则返回hello和jt节点
func (n *node) matchChildren(part string) []*node {
	nodes := make([]*node, 0)
	for _, child := range n.children {
		if child.part == part || child.isWild {
			nodes = append(nodes, child)
		}
	}
	return nodes
}

//这个参数是相当于传了一个二级指针
//如果传递的节点为根节点， 则把这个节点下的所有节点都加入list集合
func (n *node) travel(list *([]*node)) {
	if n.pattern != "" {
		*list = append(*list, n)
	}

	//把当前node的孩子都加入到list管理
	for _, child := range n.children {
		child.travel(list)
	}
}
