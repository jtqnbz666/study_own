---

---

**用户列表的信息是怎么得到的？**

(在CSessionModel模块)根据不同的用户id 从关系表IMRecentSession中获取对方的ID(可能是用户id，也可能是群组id)， 可以通过表中的type字段区分是哪种类型，表中包含更新时间，当需要更新的时候，判断这个更新时间，如果大于当前时间，则获取这条记录，每次最多获取100条，但通过这个表能得到的消息只有：1.对方id，2.对方类型，3.对方状态(在线...)，4.消息id，还需要获取这个会话的最后一条消息的信息(最后一条消息由谁发的，类型，数据)，这些内容需要在消息表中获取，(在CSessionModel::fillSessionMsg中，单聊消息调⽤CMessageModel::getLastMsg，群聊消息调⽤CGroupMessageModel::getLastMsg)所以需要查询消息表(分为普通消息表，群消息表，它们都有八张表，前者用relationid%8, 后者用groupid%8)，分别以relationid和groupid为索引，使用orderby字段排序获取最后一条消息。

**在线文件发送过程？**

msg_server收到CID_FILE_REQUEST命令，和一个IMFileReq结构数据，包含from_user_id⽂件来⾃谁，to_user_id发给谁，file_name⽂件名，file_size⽂件⼤⼩，trans_mode在线/离线模式(这个状态信息是在客户端那边就判断出来了的，但可能后边又离线了，所以后边还需要判断)，自己将这些信息封装为IMFileTransferReq结构数据(包含from_id, to_id, file_name, file_size, 比刚才多增加了一个attach_data,它记录的是当前msgserver**对应的client**的fd)， 需要分情况决定是否直接将这些信息发送给fileserver
1：若是离线文件，则直接发送给fileserver
2：若是在线状态，且to_id也在当前msgserver上，直接发送给fileserver
3：是在线状态，但to_id不在当前msgserver，则将上述的这些内容(IMFileTransferReq结构数据)封装为一个pdu_1，绑定到attach_data(它不仅可以装fd，还可以装fd+pdu)，再将attach_data封装为另一个pdu_2，一起发送(发送pdu_2即可，pdu_1在pdu_2中的attach_data里，**演示了如何pdu中套pdu，并拆解出我们需要的那个pdu**)给routeserver来查询to_id的在线状态，若成功在routesesrver中查询到了，会将to_userid的状态返回给msgserv, 若是在线状态，再发送给fileserv(和上边其他两种情况的发送过程一样)；
fileserver会生成taskID返回给msgserv，回应中主要就是增加了taskID字段，然后msgserv会把fileserv的ip和port以及taskID一起返回给**发送方client**；
同时msgserv会根据to_userid 在当前msgsev中查看是否有接收方信息，若没有，则在routeserv中，找到对方信息，**并通知接收方(⾥⾯包含了对应的file_server的ip和port地址，以及其他的必要信息)**，这样双方都有了fileserv的ip和port，会一起请求连接fileserv，不管哪方先登录上fileserv，fileserv会先查询task_id是否存在，若存在，并且双方都已经登录上fileserv，fileserv会发送一个命令到接收方(对应客户端就是弹框问你是否接收文件)，若点击接收，则接收方会发送CID_FILE_PULL_REQUEST来请求数据，fileserv进一步将命令转发给发送方，发送方再回应CID_FILE_PULL_RESPONSE给fileserv，fileserv又进一步转发给接收方。

**离线文件发送过程？**

client向msgserv请求发送离线文件，msgserv转发请求到fileserv，fileserv生成离线任务(task_id)返回给msgserv, msgserv把该task_id,以及fileserv的ip，port一起返回给client, client登录fileserv，触发fileserv的_HandleClientFileLoginReq函数，进一步进入ChangePullState(将状态从ready更改为waitingupload)，在这个函数中fileserv会向client请求需要上传的数据，此时状态为waitingupload， 那么自然就可以处理上传资源的逻辑，**fileserv会直接发送CID_FILE_PULL_DATA_REQ给上传者**(第一次发送这个命令是在_HandleClientFileLoginReq函数中)，上传者以CID_FILE_PULL_DATA_RSP回应fileserv，**fileserv调用DoRecvData**函数接收数据(对于在线，则只是检查状态， 若离线则将数据**缓存到本地**)，又继续发送CID_FILE_PULL_DATA_REQ给上传者，数据上传完毕后将状态更换为uploadend；
后续接收方client上线后会查询是否有离线文件，接着会登录fileserv，会第二次(这次一定是接收方)进入ChangePullState， 将刚才说到的uploadend更改为waitingdownload,此时状态为waitingdownload， 那么自然就可以处理下载资源的逻辑（下载者会发送CID_FILE_PULL_DATA_REQ给fileserv，**fileserv会调用DoPullFileRequest读取离线数据**直接返回给下载者）

**在线传输和离线传输的区别**？

对于在线传输，**当两方都登录上fileserv后**，fileserv会发送一个CLIENT_FILE_PEER_READY命令给**接收方**(对应到客户端就是，出现那个是否接收文件的弹框)，若接收方点击接收，则**接收方**会**主动**发起CID_FILE_PULL_DATA_REQ命令来向fileserv请求数据，fileserv会把请求转发给**发送方**，**发送方**再回复CID_FILE_PULL_DATA_RSP来返回数据给fileserv，fileserv再转发给**接收方**。

对于离线传输**，当上传者登录到fileserv后**，fileserv会**直接**发送CID_FILE_PULL_DATA_REQ给上传者(第一次发送这个命令是在_HandleClientFileLoginReq函数中)，上传者回复CID_FILE_PULL_DATA_RSP给fileserv，fileserv把这些数据缓存下来,若还没有发送完毕，则继续在这个回应中发送CID_FILE_PULL_DATA_REQ给上传者，直到数据上传完毕后，会发送CLIENT_FILE_DONE给上传者。