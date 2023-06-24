

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

