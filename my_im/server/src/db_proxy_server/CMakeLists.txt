cmake_minimum_required(VERSION 2.6)
PROJECT(db_proxy_server)


#由此可见可以命名相同的名称
AUX_SOURCE_DIRECTORY(./ SRC_LIST)
AUX_SOURCE_DIRECTORY(./business SRC_LIST)


#如果创建了build目录，同样需要多退一级
SET(PB_LIB_DIR ../../base/pb/lib/linux)    #简单的放到/usr/lib不行，必须显式写出来

SET(EXECUTABLE_OUTPUT_PATH  ./)

SET(MYSQL_INCLUDE_DIR /usr/include/mysql)
SET(MYSQL_LIB /usr/lib64/mysql)


#加上 -static 表示显式调用静态库
ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11 -DENCRYPT )

#这个相当于 -I   指定头文件路径
#经验证， 不会递归去查找这个目录下的其他目录
INCLUDE_DIRECTORIES(./ ../base  ./businiess ../base/jsoncpp ../base/pb ../base/pb/protocol ${MYSQL_INCLUDE_DIR})

#这个相当于 -L   指定库文件路径
#这里有个细节， 如果新建了build目录，cd build -> cmake .. 则这里必须要多退一级，比如./ 要更换为 ../
#而上边的那些却不LINK_DIRECTORIES(./ .. ../base ../../slog/lib ${MYSQL_LIB} )
LINK_DIRECTORIES(./  ../../base/build  ${PB_LIB_DIR} ${MYSQL_LIB} ../../base/slog/lib)

ADD_EXECUTABLE(db_proxy_server ${SRC_LIST} )

TARGET_LINK_LIBRARIES(db_proxy_server pthread base protobuf-lite mysqlclient hiredis curl slog crypto)



 
