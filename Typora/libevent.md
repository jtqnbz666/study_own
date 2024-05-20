1:**ev_next**，**ev_active_nex**t和**ev_signal_next**都是双向链表节点指针；它们是libevent对不同事件类型和在不同的时期，对事件的管理时使用到的字段。libevent使用双向链表保存所有注册的I/O和Signal事件，ev_next就是该I/O事件在链表中的位置；称此链表为“已注册事件链表”；同样ev_signal_next就是signal事件在signal事件链表中的位置；ev_active_next：libevent将所有的激活事件放入到链表active
list中，然后遍历active list执行调度，ev_active_next就指明了event在active list中的位置；

2:evsel**和**evbase这两个字段的设置可能会让人有些迷惑，这里你可以把evsel和evbase看作是类和静态函数的关系，比如添加事件时的调用行为：evsel->add(evbase,  ev)，实际执行操作的是evbase；这相当于class::add(instance, ev)，instance就是class的一个对象实例。
 evsel指向了全局变量static const struct eventop *eventops[]中的一个；

libevent将系统提供的I/O  demultiplex机制统一封装成了eventop结构；因此eventops[]包含了select、poll、kequeue和epoll等等其中的若干个全局实例对象。evbase实际上是一个eventop实例对象