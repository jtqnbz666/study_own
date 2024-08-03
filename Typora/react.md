

8.常用路由设计方法

~~~react
1.创建上下文
const serverContext = createContext();
2.自定义一个组件ProvideServer，并设置上下文提供一个全局的server对象，该组件的子组件可以通过上下文访问到这个server对象
export function ProvideServer({children}) {
    const server = useProvideServer();
    return <serverContext.Provider value={server}>{children}</serverContext.Provider>;
}
3.给该自定义组件设置一个子组件router，router也就是第二点中的children
<ProvideServer>
    <RouterProvider router={router}/>
</ProvideServer>
4.第三点的router子组件可以通过此方法访问到server对象
export const useServer = () => {
    return useContext(serverContext);
};

总结：ProvideServer 组件是一个上下文提供者，它使用 serverContext.Provider 将 server 对象传递给其子组件。
useProvideServer 是一个自定义钩子，用于获取或创建 server 对象。
通过上下文机制，子组件可以方便地访问和使用 server 对象。
~~~

7.提示框

~~~react
Modal.error({
                title: '请求用户列表失败',
                content: res.err,
                hasCancel: false,
            });
~~~

6.发送的时候需要发送数字

~~~react
 const res = await server.deleteUserCache({
            userId: Number(userId), # 不管go那边用什么类型的数字都可以
            displayId: displayId,
            phone: phone,
        }); 
~~~

5.弹窗的日志

~~~react
window.alert(`成功重置为 ${res.admin.name}`);
# 注意变量的打印方式, 这是接收到返回的时候， 如果直接是react里面定义的常量就可以直接 window.alert(value);
~~~

4.useRef

~~~react
// case1:
const [count, setCount] = useState(0)
const preCount = useRef()

function handleClick() {
  preCount.current = count
  setCount(count + 1)
}

return (
	<div>
  	<p>最新的count: {count}</p>
    <p>上次的count: {preCount.current}</p>
    <button onClick={handleClick}>增大count</button>
  </div>
)

//case2:

const inputRef = useRef(null)
function handleClick() {
  inputRef.current.focus()
}

return (
  <div>
  	<input type="text" ref={inputRef} />
    <button onClick={handleClick}>按钮</button>
  </div>
)
~~~



3.useReducer

~~~react

//计数器
function countReducer (state, action) {
  switch (action.type) {
      // 这里的action说的是下边的dispatch传递的参数
    case "increment":
      return state + 1
    case "decrement":
      return state - 1
    default:
      throw new Error()
  }
}
export default function App () {
const [state, dispatch] = useReducer(countReducer, 0)
const handleIncrement = () => dispatch({type: "increment"})
}
~~~

2.useEffect的使用

~~~react
import React, { useState, useEffect } from 'react';
function Example() {
  const [data, setData] = useState(null);
  useEffect(() => {
    fetchData();
  }, []); //第二个参数是数组，空数组表示只在组件挂载时执行，用于指定哪些状态或属性变化时需要重新执行 useEffect 中的函数 
  
  const fetchData = () => {
    // 模拟异步获取数据
    setTimeout(() => {
      setData('Hello, world!');
    }, 2000);
  };
  return <div>{data}</div>;
}


//场景举例:比如多页的数据，把页的属性放到这里面。你每点击下一页的时候，就可以利用触发useEffect里的内容去请求下一页的数据，比如
const [pagination, setPagination] = useState({currentPage: 1, pageSize: 20});

useEffect(() => {
getDubbingList({limit: pagination.pageSize, offset: (pagination.currentPage - 1) * pagination.pageSize})
  .then((res) => {
    if (!res.dubbing) {
      res.dubbing = [];
    }
    setData(res.dubbing.map((v) => ({...v, dubbingPath: `http://hoxigames.oss-cn-beijing.aliyuncs.com/assets/dubbing/${v.dubbingPath}`})));
    setTotal(res.total);
  });
}, [pagination]);
~~~

2.useState的使用

~~~react
// 计数器
import React, { useState, useEffect } from 'react';
const [count, setCount] = useState(0)
const handleIncrement = () => setCount(count + 1)
const handleDecrement = () => setCount(count + 1)
return (
	<div style={{ paddind: 10}}>
  	<button onClick={handleIncrement}>-</button>
    <span> {count} </span>
    <button onClick={handleDecrement}>+</button>
  </div>
)
~~~



codecase:

~~~react
1.例如pagination 对象最初看起来像这样
const [pagination, setPagination] = useState({currentPage: 1, pageSize: 20});
const pagination = {
  currentPage: 1,
  pageSize: 10,
  total: 100
};
如果调用 setPagination({...pagination, currentPage: 2})，那么 pagination 对象将会被更新为：
{
  currentPage: 2,
  pageSize: 10,
  total: 100
}
...pagination 这一部分就是将 pagination 对象的所有属性复制到新对象中，然后 currentPage 属性被设置为 2。
~~~



基础语法

~~~react
4.常用元素: <Form> 和 <div>是 HTML 中的两种不同的元素，<Form> 用于创建表单，而<div> 用于创建内容分隔符；
Modal（弹窗）、Switch（开关按钮）和Table（表格）
span是一个行内元素，通常用于对文本的一部分进行修饰、高亮或添加特定的样式，p是一个块级元素
2.jsx(JavaScript XML)意思是javascript 和 html的结合的一种模版语法
function App(){
  // 里面嵌入html的语法
}
1.return的时候带上(),返回单行可以不用
~~~

长篇基础

~~~react
div：div是HTML中的一个元素，也是React中最常用的一个元素。它本质上是一个没有样式和默认行为的块级容器，用于在页面中创建和组织布局。

form：form也是HTML中的一个元素，用于创建一个包含输入字段、按钮以及其他交互控件的表单。在React中，form组件通常用于处理表单数据的提交和验证，以及处理用户输入的逻辑。

modal：modal是一种在Web应用中常见的弹出窗口或对话框。它常用于显示额外的内容或用于确认用户操作。当你需要显示一个覆盖在页面上的浮动窗口，用于提供额外的信息或与用户进行交互时，可以使用modal组件。

switch：switch是一个开关组件，通常用于表示二进制选项（如开/关、启用/禁用等）的状态切换。用户可以通过点击或切换按钮来改变其状态，以便在应用程序中进行相应的操作。

table：table是HTML中用于呈现表格结构的元素。在React中，table组件通常用于展示和操作数据，通常与tr（表格行）和td（表格数据）等其他元素一起使用，用于构建具有各种样式和功能的表格。

span：span是一个行内元素，用于在文本中包裹一个特定的区域或应用样式。它通常用于对文本的一部分进行修饰、高亮或添加特定的样式。

p：p是一个块级元素，用于定义段落或文本块。它通常用于组织和表示独立的文本段落，其内容会在视觉上以段落的形式进行呈现。
~~~

npx create-react-app my-app创建项目
react的组件就是函数的形式， 通过export default进行导出
brew install node
npm（node package manager）
npm start 运行
npm run build 修改后重新编译
