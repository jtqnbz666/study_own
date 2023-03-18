#!/bin/sh 
#不要有空格， 配置文件不一样
SRC_DIR=.
DST_DIR=./gen

#C++
mkdir -p $DST_DIR/cpp
protoc -I=$SRC_DIR --cpp_out=$DST_DIR/cpp/ $SRC_DIR/*.proto 
