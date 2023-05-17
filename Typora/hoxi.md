Generate.go:根据环境配置文件，比如version.sh(deploy-tool/version.sh和deploy-tool/config/target/version.sh)和evn.sh(deploy-tool/config/env.sh)，配合go中的template方法，生成四套环境(./deploy-tool/conf下)。 比如service.ini这些配置文件就来源于这四套环境

使用到公网ip的目前只有battle-server_manager.ini ,其他都是用的local IP

Fluentd是一个开源的通用日志采集和分发系统





project328中global下的一些参数具体的值是通过serivce下的evn.sh配置的 



很多东西(project328和userdataservice中的gen/conf下的gen系列都来自于config-tool/)



proto文件存放在config-tool/protos/rawdata下,  生成的文件其实是在config-tool/gen/golang/project328-server/gen/conf/rawdata，每个服务启动的时候都有一个service.ini文件，里面有一个CardConfig路径的配置，通过它进一步会把所有生成的go.pb文件弄到服务对应的/gen/rawdata下（直接去project328的server.go看service.InitCardGameConfig()，就是在做这个事情）

通过excel表格倒出来的数据会生成.pb和.json文件，生成的文件在服务对应的/ci/asset/card-game-config下

excel文件都在config-tool/excel下

通信实体或者一些其他的实体都在hoxi-server/interfaces下