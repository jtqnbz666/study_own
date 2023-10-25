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

