对于写xml的操作主要掌握以下几点

import  xml.dom.minidom as minidom先包含它
这里面的as 表示取别名

创建文档(两种方式)

~~~c
第一种
doc=minidom.getDOMImplementation().createDocument(None, 'root', None)
然后获取根节点
root = doc.documentElement
    
第二种
doc = minidom.Document()
root = doc.createElement('record')
doc.appendChild(root)

~~~



创建节点：
element = doc.createElement(name)

给创建的这个节点添加文本：element.appendChild(doc.createTextNode('jt'))

设置属性：
element.setAttribute('age', '20') 如果是多个属性就多写几行

添加节点(比如加给root节点)
root.appendChild(element)

