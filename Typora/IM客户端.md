## Login登录模块

在Moudles项目中。 在Login文件夹中找到LoginDialog.cpp(_DoLogin()) ， 在TcpClient文件夹中找到 TcpClientNodule.cpp(doLogin())模块.

在LoginDlalog中213行可以看到创建了一个LoginOperation(继承自ICallbackOpertaion)对象，将这个对象当作一个任务放入了一个list待执行队列中， 并为它注册了一个回调函数(不只是登录模块是这样，其他的任务也是以继承ICallbackOpertaion类的方式， 将其子类实例化对象放入这个list链表中)

在LoginOperation.cpp中48行可以看到doLogin的具体调用，是在processOpertion中调用它的， processOpertion是一个重写的函数， 不同的Operation类(这里是LoginOperation)对应者不同功能的processOpertion。