# MarkDown小白使用手册

## 1.符号的使用

#### No1.1删除线 shanchuxian

//左右两边都加两个~就行，也可以直接用快捷键Alt＋Shift+5即可

#### No1.2分割线

------

------

//上边那根是通过三个‘-’号形成的

//下边那条分割线是通过连续三个‘*’形成的。

#### No1.3下划线

//一般使用快捷键 Ctrl+U即可

## 2.图片的插入以及位置大小的设置

既可插入本地图片也可直接插入网络图片

格式为 ！【图片名称】（图片链接）

- 位置的设置：一般用到了居中
  格式为！【图片名称】（图片链接#pic_center)
- 大小的设置：
  格式为！【图片名称】（图片链接 大小x大小）
  **这里一定注意图片链接和大小设置中间需要一个空格符号嗷**

#### No2.1 插入一张本地图片

![在这里插入图片描述](https://img-blog.csdnimg.cn/8b002f02abd146bd9f9eac029a6bb9db.jpg)

#### No2.2 插入一张网络图片

![img](https://img-blog.csdnimg.cn/img_convert/1205f43cae9a3c648463d7f1d101d4ab.webp?x-oss-process=image/format,png)

## 3.超链接

### No3.1使用方法

跟插入图片相比较，只是舍去了一开始的 感叹号’ ！‘。

[超链接的名称]（括号里面放链接的地址）//注意括号是英文的

### No3.2实际运用

[点击进入MarkDown教学视频网站](https://editor.csdn.net/md/[[B站搜索狂人说]Java零基础学习视频通俗易懂_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV12J41137hu?p=6))

## 4.列表

### No4.1有序列表

1.A

2.B

3.C

#### 使用方法

//输入一个数字 接着 输入一个 ’ . '再按一下空格即可实现。

1. 
2. 

### No4.2无序列表

#### 格式各异

- A
- B
- C
- D
- 西海岸
  - 缅甸北部
    - 印度森林

#### 使用方法

//输入一个减号’ - ‘，再按一下空格即可实现。

- H
- G
- F

## 5.表格的使用

### No5.1表格的创建方法（局右局左居中）

| 名字 | 性别 | 生日       |
| ---- | ---- | ---------- |
| 江涛 | 男   | 2002.02.02 |
| 王慧 | 女   | 2001.09.21 |
| 理想 | 男   | 2002.08.23 |

- 具体实现方法：

1. |:----| 这样就是居左的写法，长度由自己决定
2. |----：| 居右
3. |:----：| 居中

- |–|--|–| 这样即可完成一个表格的创建，也可以直接使用快捷键的方式创建

如果无法使用，启用源代码模式，把中间的空格移除即可。

```c++
#include <iostream>
#include <string>
using namespace std;
int main ()
{
    
    
    system("pause");
    return 0;
}
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
int main()
{
    
    
    system("pause");
    return 0;
}
```

## 6.代码的编写

### 方法一

使用英文的```，然后在后边输入想要使用的编写语言。

### 方法二

使用~~~， 同理接着输入想要使用的编译语言。

```c
#include <iostream>
using namespace std;
int main()
{

system("pause")
return 0;
}
```

## 7.如何实现(已办和待办)

### No1.-空格[空格]空格即可实现（代办）

-  任务待完成

### No2.-空格[x]空格即可实现（已办）

-  任务已经完成

## 8.引脚的使用方法

- 注意：此处使用的一切符号均需要使用英文符号。！！！
- 具体使用方法：[^数字]
- 换行再输入 [^数字]:引脚的名字。
- 还有一种就是[^数字]:[引脚名字]（链接地址）

1. 帅哥美女的图片 [1](https://editor.csdn.net/md/?articleId=120743910#fn1)

1. 小弟的图片[2](https://editor.csdn.net/md/?articleId=120743910#fn2)

1. 3.普通类型[3](https://editor.csdn.net/md/?articleId=120743910#fn3)

------

1. [点击此处查看帅哥美女图片](https://img-blog.csdnimg.cn/6092ec9055604334b7f03882f710ad8d.jpg) [↩︎](https://editor.csdn.net/md/?articleId=120743910#fnref1)
2. [点击查看小弟的图片](https://img-blog.csdnimg.cn/0bcde9d3abbb49c5b16e6a8a72de6ff8.jpg) [↩︎](https://editor.csdn.net/md/?articleId=120743910#fnref2)
3. 普通的引脚，没有相应的链接。 [↩︎](https://editor.csdn.net/md/?articleId=120743910#fnref3)