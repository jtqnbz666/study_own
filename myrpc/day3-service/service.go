import (
	"go/ast"
	"log"
	"reflect"
	"sync/atomic"
)

type methodType struct {
	method    reflect.Method //方法本身
	ArgType   reflect.Type   // 第一个参数的类型
	ReplyType reflect.Type   //第二个参数的类型， 必须是指针， 因为传参传指针才能真正修改对象，不然只是一个拷贝，外界就不知道具体值
	numCalls  uint64         // 后续统计方法调用次数的时候会用到
}

func (m *methodType) NumCalls() uint64 {
	return atomic.LoadUint64(&m.numCalls)
}

// 对接收到的内容进行反射
func (m *methodType) newArgv() reflect.Value {
	var argv reflect.Value
	//第一个参数可能是指针， 也可能只是普通对象
	if m.ArgType.Kind() == reflect.Ptr { // 如果是指针对象就要用到 Elem()
		argv = reflect.New(m.ArgType.Elem())
	} else {
		argv = reflect.New(m.ArgType).Elem()
	}
	return argv
}

func (m *methodType) newReplyv() reflect.Value {
	//第二个参数必须是指针类型
	replyv := reflect.New(m.ReplyType.Elem())
	switch m.ReplyType.Elem().Kind() {
	case reflect.Map:
		replyv.Elem().Set(reflect.MakeMap(m.ReplyType.Elem()))
	case reflect.Slice:
		replyv.Elem().Set(reflect.MakeSlice(m.ReplyType.Elem(), 0, 0))
	}
	return replyv
}

type service struct {
	name   string                 //映射的结构体的名字，比如 student是一个结构体对象， 那么name就是student
	typ    reflect.Type           //type 是结构体的类型
	recv   reflect.Value          // receiver是结构体的实例本身， 作为调用时的第0个参数
	method map[string]*methodType //映射方法
}

func newService(recv interface{}) *service {
	s := new(service)
	s.recv = reflect.Value(recv)
	s.name = reflect.Indirect(s.recv).Type().Name()
	s.typ = reflect.TypeOf(recv)
	if !ast.IsExported(s.name) {
		log.Fatalf("rpc server: % 不是一个正确的服务名", s.name)
	}
	s.registerMethods()
	return s
}

func (s *service) registerMethods() {
	s.method = make(map[string]*methodType)
	for i := 0; i < s.typ.NumMethod(); i++ {
		method := s.typ.Method(i)
		mType := method.Type
		if mType.NumIn() != 3 || mType.NumOut() != 1 {
			continue
		}
		if mType.Out(0) != reflect.TypeOf((*error)(nil)).Elem() {
			continue
		}

		argType, replyType := mType.In(1), mType.In(2)
		if !isExportedOrBuiltinType(argType) || !isExportedOrBuiltinType(replyType) {
			continue
		}

		s.method[method.Name] = &methodType{
			method:    method,
			ArgType:   argType,
			ReplyType: replyType,
		}

		log.Printf("rpc server: 注册了%s . %s\n", s.name, method.Name)
	}
}

func isExportedOrBuiltinType(t reflect.Type) bool {
	return ast.IsExported(t.Name()) || t.PkgPath() == ""
}

func (s *service) call(m *methodType, argv, replyv reflect.Value) error {
	atomic.AddUint64(&m.numCalls, 1)
	f := m.method.Func
	returnValues := f.Call([]reflect.Value{s.recv, argv, replyv})
	if errInter := returnValues[0].Interface(); errInter != nil {
		return errInter.(error)
	}
	return nil
}