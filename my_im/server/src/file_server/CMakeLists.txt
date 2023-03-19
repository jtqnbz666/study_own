cmake_minimum_required(VERSION 2.6)
PROJECT(fileserver)

AUX_SOURCE_DIRECTORY(./ SRC_LIST1) 
#AUX_SOURCE_DIRECTORY(../base/pb/protocol SRC_LIST2)

SET(PB_LIB_DIR ../../base/pb/lib/linux)

ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(./ ../ ../base/pb ../base ../base/pb/protocol )
LINK_DIRECTORIES(./ ../../base/build ${PB_LIB_DIR} ../../base/slog/lib)

ADD_EXECUTABLE(file_server ${SRC_LIST1} )
TARGET_LINK_LIBRARIES(file_server pthread base protobuf-lite uuid slog)

