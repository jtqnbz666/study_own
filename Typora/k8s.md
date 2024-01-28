通用技巧

~~~shell
4.pod理解为一个虚拟机，也就是说一个pod中的两个容器不可能是同一个端口
3.-A获取所有命名空间的资源
2.--watch 动态看资源的变化
1.进入pod比如exec -it 必须接上--，比如 kubectl exec -it mytest -- mysql -u root -p
~~~

删除操作

~~~shell
1.把副本集删了不会影响deploy, 但把deploy删了，pod也会被删
~~~



版本回滚(比如过程更新了镜像版本又想回退回去)

~~~shell
kubectl rollout history deploy/mytest-deployment
# 会得到
deployment.apps/mytest-deployment
REVISION  CHANGE-CAUSE
1         <none>
2         <none>

kubectl rollout history deploy/mytest-deployment --revision=1 # 看详情
# 然后执行
kubectl rollout undo deploy/mytest-deployment --to-revision=1 # 回滚

# 回滚本质上是通过副本集(rs)去回滚，就继续用之前的副本集来构建deployment和pod了


~~~

滚动更新(更新镜像)

~~~shell
# 说明:修改的是deployment对象
kubectl set image deploy/nginx-deploy nginx=nginx:1.23

# 过程:
会创建一个新的副本集，然后用这个新的副本集生成之前副本集的个数，之前那个副本集不会删除只是没有使用旧的副本集生成pod了， 通过kubectl get rs 可以看到副本集信息
~~~

副本数量调整

~~~shell
kubectl scale deployment nginx-deploy --replicas=5 # 增加减少都行
# 动态调整
kubectl autoscale deployment/nginx-auto --min=3 --max=10 --cpu-percent=75
# 查看自动缩放
kubectl get hpa
# 删除自动缩放
kubectl delete hpa nginx-deployment
~~~

创建deployment（可简写为deploy)

~~~shell
kubectl create deployment nginx-deploy --image=nginx:1.22 --replicas=3
kubectl delete pod 删除任意一个，他会自愈(重新创建一个，保证有副本集数量为3)， 跟下边这个查看副本集是关联的
~~~

查看副本集

~~~shell
kubectl get replicaSet #replicaSet可缩写为rs
比如结果是nginx-deploy-85bc6c755并且有两个副本集， 使用kubectl get pod 就看到的是nginx-deploy-85bc6c755-A和nginx-deploy-85bc6c755-B
~~~

第一个测试的mysql的pod

~~~yaml
比如简单的nginx可以直接用命令:
kubectl run myNginx --image=nginx
但因为mysql涉及到密码的设置，kubectl不支持直接-e, 所以需要写个yaml

部署
kubectl apply -f mysql-pod.yaml
进入
kubectl exec -it mytest -- mysql -u root -p # -- 不可少

yaml内容如下
apiVersion: v1
kind: Pod
metadata:
  name: mytest
spec:
  containers:
    - name: mysql
      image: mysql
      env:
        - name: MYSQL_ROOT_PASSWORD
          value: "123456"
~~~

进入pod中

~~~
kubectl exec -it pod名 -- /bin/bash
~~~

查看pod的更详细信息(比如ip地址和镜像名)

~~~shell
kubectl get pod -o wide  #类比-o yaml,-o是输出格式
#kubectl get --help 可查看更多
~~~

Service,deployment,pod的关系

~~~
Deployment 负责管理 Pod 的创建和副本数量，而 Service 则提供了稳定的网络访问方式，使得可以通过 Service 访问到由 Deployment 管理的一组 Pod。
~~~

修改cm里面的fluentd-config配置

~~~shell
# 先删除旧的
kubectl delete cm fluentd-config
# 再创建新的
./create-config-map.sh

# create-config-map.sh 脚本内容
#!/bin/sh

kubectl create configmap fluentd-config -n default --from-file=./fluent.conf
~~~

**现在对用户是一个svc，后面是10个pod**

创建一个service

~~~shell
kubectl apply -f fluentd.yaml
具体的yaml内容
~~~

删除一个服务

~~~shell
先kubectl get svc -n default #比如删除fluentd
kubectl delete service fluentd -n default
~~~

删除一个pod

~~~shell
# 根据pod的名字删除（相当于重启，因为pod的个数都是配置好的，如果没有了足够的数量就会启动新的节点
kubectl get pod -n default #得到fluentd-db9bfd9f9-qlfrp
kubectl delete pod fluentd-db9bfd9f9-qlfrp -n default
~~~

编辑配置文件

~~~shell
kubectl edit cm -n default xxx(比如fluentd-config)

#改之前可以先备份一下
kubectl get cm -n default fluentd-config -o yaml > fluentd-config-backup.yaml
#如果改错了就恢复回去
kubectl apply -f fluentd-config-backup.yaml
~~~

备份配置文件

~~~shell
# -o yaml 可以把配置以yaml的格式打印出来
# 比如cm的， svc也是一样
kubectl get cm -n default fluentd-config -o yaml > fluentd-config-backup.yaml
~~~

查看配置文件内容

~~~shell
kubectl describe cm -n default fluentd-config
~~~

查看配置文件有哪些

~~~shell
kubectl get cm -n default （cm:configmap
~~~

查看某个pod的详细信息

~~~shell
kubectl describe pod -n taptap  taptap-project328-server-7ccbf6fc5b-4mxgh
~~~

查看svc的信息， 并且以命名空间区分

~~~shell
#服务列表
kubectl get svc -n taptap
~~~

获取所有pod

~~~shell
kubectl get pod -A   # 注意一定要是-A 而不是 -a, 不管是pod还是其他资源都一样
~~~

获取pod信息

~~~shell
#Pod 列表
kubectl get pod -n taptap 
~~~

服务列表和Pod 列表区别

~~~shell
概念：
Pod 是 Kubernetes 中的最小部署单元，它可以包含一个或多个容器。Pod 用于运行应用程序或服务，并提供一组共享的网络和存储资源。
服务是 Kubernetes 网络抽象的一种方式，用于公开应用程序或服务。服务为 Pod 提供了稳定的网络访问地址，并负责在多个 Pod 之间进行负载均衡。
功能：
Pod 主要负责运行应用程序或服务，并提供其所需的计算资源、存储资源和网络连接等。
服务主要负责提供稳定的网络入口和负载均衡功能，使应用程序或服务能够在集群内或集群外可访问。
关系：
服务通常关联多个 Pod。服务通过标签选择器（Selector）关联一组具有相同标签的 Pod，从而将流量分发到这些 Pod。
Pod 可以独立存在，也可以是服务的一部分。服务通常使用 Pod 的 IP 地址和端口来提供访问入口。
总结： 服务列表和 Pod 列表在 Kubernetes 中担任不同的角色和功能。Pod 是实际运行应用程序或服务的基本单位，而服务提供网络抽象和负载均衡的能力，使应用程序能够通过稳定的入口地址对外提供服务。服务通常关联多个具有相同标签的 Pod，以提供可扩展和可靠的访问机制
~~~



查看pod的内存占用

~~~shell
kubectl top pods -n taptap
~~~

列出pod包含的容器名字

~~~shell
kubectl get pod test-leader-board-service-7dd6bf4556-xv8lp -o jsonpath='{.spec.containers[*].name}'

结果：test-leader-board-service ta-logbus

# 解释
{}: 表示开始和结束一个 JSONPath 查询语句。
.spec.containers: 表示访问 Kubernetes 对象中的 .spec.containers 字段，该字段包含了 Pod 中的容器列表。
[*]: 表示匹配 .spec.containers 中的所有元素。
.name: 表示访问每个容器对象中的 name 字段。
~~~

查看lbs这个pod中指定容器的日志

~~~shell
kubectl logs test-leader-board-service-7dd6bf4556-xv8lp -c test-leader-board-service(这里只能用容器名字，不能是容器id，容器id用于标识容器实例)
~~~

查看lbs这个pod的所有容器的日志

~~~shell
kubectl logs test-leader-board-service-7dd6bf4556-xv8lp --all-containers
~~~

从日志的最后10行开始看，并且动态的看

~~~shell
kubectl logs -f --tail=10 test-leader-board-service-7dd6bf4556-xv8lp --all-containers
~~~

查看uds的pod的所有容器

~~~shell
kubectl describe pod -n test test-user-data-service-58858fdf8f-c6lgv | grep -E "Containers:|Container ID:"

结果：
Containers:
    Container ID:   containerd://247a4882e36253298f04bd19d737ea62f10242e6d99a4b294b69521b0c95163e
    Container ID:   containerd://76a7858511f78f618204805ae12b2be967fb15490e927b639544a25020cc1d77
~~~



学习地址：课件地址：https://www.yuque.com/wukong-zorrm/qdoy5p
