**现在对用户是一个svc，后面是10个pod**

查看某个pod的信息

~~~
kubectl describe pod -n taptap  taptap-project328-server-7ccbf6fc5b-4mxgh
~~~

查看svc的信息， 并且以命名空间区分

~~~
 kubectl get svc -n taptap
~~~

获取pod信息

~~~
kubectl get pod -n taptap
~~~

查看pod的内存占用

~~~
kubectl top pods -n taptap
~~~

列出pod包含的容器名字

~~~
kubectl get pod test-leader-board-service-7dd6bf4556-xv8lp -o jsonpath='{.spec.containers[*].name}'

结果：test-leader-board-service ta-logbus
~~~

查看lbs这个pod中指定容器的日志

~~~
kubectl logs test-leader-board-service-7dd6bf4556-xv8lp -c test-leader-board-service(这里只能用容器名字，不能是容器id，容器id用于标识容器实例)
~~~

查看lbs这个pod的所有容器的日志

~~~
kubectl logs test-leader-board-service-7dd6bf4556-xv8lp --all-containers
~~~

从日志的最后10行开始看，并且动态的看

~~~
kubectl logs -f --tail=10 test-leader-board-service-7dd6bf4556-xv8lp --all-containers
~~~

查看uds的pod的所有容器

~~~
kubectl describe pod -n test test-user-data-service-58858fdf8f-c6lgv | grep -E "Containers:|Container ID:"

结果：
Containers:
    Container ID:   containerd://247a4882e36253298f04bd19d737ea62f10242e6d99a4b294b69521b0c95163e
    Container ID:   containerd://76a7858511f78f618204805ae12b2be967fb15490e927b639544a25020cc1d77
~~~



kubetcl get pods -

Tail -f

日志服务sls找到 ali啥的

hanshu计算fc
