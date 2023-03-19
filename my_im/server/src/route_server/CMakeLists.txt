cmake_minimum_required(VERSION 2.6)
PROJECT(routeserver)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ./)
SET(PB_LIB_DIR ../../base/pb/lib/linux)

SET(MYSQL_INCLUDE_DIR /usr/include/mysql)
SET(MYSQL_LIB /usr/lib64/mysql)

ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(./ ../base ../base/pb/protocol ../base/jsoncpp/ ../base/pb  ${MYSQL_INCLUDE_DIR})
LINK_DIRECTORIES(./ ../../base/build ${PB_LIB_DIR} ../../base/slog/lib  ${MYSQL_LIB})

ADD_EXECUTABLE(route_server ${SRC_LIST} ../db_proxy_server/DBPool.cpp ../db_proxy_server/CachePool.cpp)
TARGET_LINK_LIBRARIES(route_server base slog protobuf-lite pthread mysqlclient hiredis)



