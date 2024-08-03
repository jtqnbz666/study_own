适配问题

~~~
任何物体都要setScale(SCALE_FIT)来做适配，因为designSize(设计大小）和deviceSize(设备实际大小)的值不一样。
对父节点执行setScale，则父节点下的所有子节点都不需要setScale, 但如果图片过小还是可以通过setScale做拉伸。
~~~

loginButton->setCapInsets(Rect(62, 21, 60, 20));

~~~
常用语拉升一张图片， 参数对应x,y,宽,高，以切下来的这块为中心拉升图片
~~~

editBoxTextChanged

~~~
检测是哪个输入框有变化，每个editBox都有一个tag，if (editBox->getTag() == 0) {
~~~

