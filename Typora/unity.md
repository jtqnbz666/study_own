

**unity和mfc有一个区别就是mfc中的一个画面的控件都在同一个类中操作， 而unity可以把控件绑到别的画面中**，可以这样想， mfc都是以类为单位， 而unity中以场景为单位，并且场景里面可以嵌套(就像mainscenemanager里面包含了所有的场景)

宏观思维，如果是别的panel调用其他panel就通过uimanager.instance去调用(因为一开始就把所有的子panel都初始化好了)， 如果是panel内的panel，就可以直接使用了， 其实比较大的panel比如lottery和mail，对于manager来说它们就是子panel。lottery和mail里面又可以有各自的子panel。 gameobject.setActive(true)就是显示的意思

打开一个panel的时候，会默认去执行一次它的OnClose，关闭的时候会默认去执行一次它的OnClose，如果在OnClose里面又写了CloseCurrentUI()，则会把主界面一起关掉，目前来看，setactive(false)不会去执行OnClose

1.将物体串成一个链表

~~~c#
  while (_fakePlayers.Count < guideState.GameList.Count)
  {
      var player = Instantiate(_fakePlayersTemplate, _fakePlayersGoParent);
      _fakePlayers.Add(player);
  }
//_fakePlayersTemplate是某一个panel中的节点模版
//节点模版就是被_fakePlayersGoParent包含的
//这样一来，_fakePlayers链表与panel就关联上了，对_fakePlayers设置就能对panel产生影响。

~~~



游戏对象和组件是一对多的关系

一个立方体对象，按住q可以移动场景整体，w是拖动物体， 按住e可以旋转

场景漫游：按住鼠标右键，wsad前后左右， q下，e上， 加上shift健的话能更快的场景漫游

qwer是四个常用的工具，q:手型工具(视角平移) w:移动工具 e:旋转工具 r:缩放工具， t(2D缩放)

左手坐标系：大拇指：x， 食指：y， 中指：z

unity可以导入和导出包(一些资源：任务模型，脚本等)

点击一个物体的时候坐标轴不一定在物体的中心， 因为它可能有子物体，会计算出一个相对位置作为中心

unity模型本质是网格(三角形的)，之所以能显示不同的颜色是因为有材质球，新建一个立方体是白色的是因为它的材质球是白色的，可以新建一个，在assert右键创建一个材质，改为自己的色彩，拖动到立方体身上，人物模型是五颜六色的是因为它的材质球是五颜六色的

如果需要场景和游戏看到的内容一致，就选中相机，点击gameobject然后选择对齐视图(这是单次的， 可以使用快捷键 command+shift+f)

gizmos:小物件，控制比如摄像机和灯光的显示状态

可以给gameobject加一个刚体(rigidbody)的组件, 那么它就会受重力影响。

比如你创建了一个平面对象(plane)， 即使你改变了它的网格造型， 它还是有一个平面的效果在那（表现就是我的立方体依然被那个平面对象拦住了，尽管改变形状之后看起来没拦住）

command + f2 重命名

Prefab(减少重复修改)：如果需要同时修改多个类似的造型。 可以把它创建为一个prefab，从场景拖动到project就成prefab了， 1. 此时可以在project中点open打开它，对它修改就可以对每个**由它复制出来**的对象产生修改。2.也可以直接在hierarchy中修改，然后单点这个对象可以看到oveerides，点击选择reply all即可对所有的对象都生效

inspector属性可以点击🔒暂时固定然后新开一个面板，每一个属性的右边都有三个点，可以找到复制黏贴选项进行copy

添加脚本，可以直接把脚本拖动到对应的组件上， 也可以点击addcomponent搜索脚本进行添加

UI:

~~~
Text对象：
UI中的reacttransform对应的就是3D对象中的transform
UI的对象可以看到rect transform，以Text类型为例，那个框框的黄点对应的是canvas的坐标参照点，可以通过设置Text对象的pivot调整Text对象自己的中心点，0.5和0.5表示Text对象自己的正中间， 1.0，1.0表示右上角。(需要把文字放边角上的时候可以用下，比如把文字放到右上角对齐，那么就把pivot设为1.0，1.0，Pox X, Pox Y都设置为0，则刚好在右上角内部),一般默认都是以canvas的中心点为参照)
Image对象：
一开始是无法使用到场景中的， 需要点击图片在texture type中设置为sprite(2D and UI) 才可以使用
button对象：
创建一个button对象， 它自带了On Click接口，可以直接拖拽一些物体上去进行控制， 但更多的是使用脚本操作更多物体
toggle对象：
是一个可选框(选择或者不选择)，和button一样提供了
~~~

