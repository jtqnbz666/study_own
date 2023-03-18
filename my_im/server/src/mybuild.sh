#!/bin/bash
build() {
    echo "#ifndef __VERSION_H__" > base/version.h
    echo "#define __VERSION_H__" >> base/version.h
    echo "#define VERSION \"$1\"" >> base/version.h
    echo "#endif" >> base/version.h

    CURPWD=$PWD	

    for i in   slog base route_server msg_server http_msg_server file_server db_proxy_server msfs login_server ; do     
         cd $CURPWD/$i/build
         cmake ..
         make
         if [ $? -eq 0 ]; then
             echo "make msfs successed";
         else
             echo "make msfs failed";
             exit;
         fi
    done
     

    cd $CURPWD

    cp base/build/libbase.a /lib/
    cp base/slog/lib/* /lib/

    #mkdir base/slog/lib
    cp slog/slog_api.h base/slog/
    cp slog/build/libslog.so base/slog/lib/

    mkdir -p ../run/login_server
    mkdir -p ../run/route_server
    mkdir -p ../run/msg_server
    mkdir -p ../run/file_server
    mkdir -p ../run/msfs
    mkdir -p ../run/http_msg_server
    mkdir -p ../run/db_proxy_server

    cp login_server/build/login_server ../run/login_server/

    cp route_server/build/route_server ../run/route_server/

    cp msg_server/build/msg_server ../run/msg_server/

    cp http_msg_server/build/http_msg_server ../run/http_msg_server/

    cp file_server/build/file_server ../run/file_server/

    cp db_proxy_server/build/db_proxy_server ../run/db_proxy_server/

    cp msfs/build/msfs ../run/msfs/

    cp tools/daeml ../run/
    
    build_version=im-server-$1
    build_name=$build_version.tar.gz
    if [ -e "$build_name" ]; then #这里注意要有间隙 -e 这里表示如果存在则先删除
        rm $build_name
    fi

    mkdir -p ../$build_version
    mkdir -p ../$build_version/login_server
    mkdir -p ../$build_version/route_server
    mkdir -p ../$build_version/msg_server
    mkdir -p ../$build_version/file_server
    mkdir -p ../$build_version/msfs
    mkdir -p ../$build_version/http_msg_server
    mkdir -p ../$build_version/db_proxy_server
    mkdir -p ../$build_version/lib

    cp login_server/build/loginserver.conf ../$build_version/login_server/
    cp login_server/build/login_server ../$build_version/login_server/

    cp route_server/build/route_server ../$build_version/route_server/
    cp route_server/build/routeserver.conf ../$build_version/route_server/

    cp msg_server/build/msg_server ../$build_version/msg_server/
    cp msg_server/build/msgserver.conf ../$build_version/msg_server/

    cp http_msg_server/build/http_msg_server ../$build_version/http_msg_server/
    cp http_msg_server/build/httpmsgserver.conf ../$build_version/http_msg_server/

    cp file_server/build/file_server ../$build_version/file_server/
    cp file_server/build/fileserver.conf ../$build_version/file_server/
    
    cp db_proxy_server/build/db_proxy_server ../$build_version/db_proxy_server/
    cp db_proxy_server/build/dbproxyserver.conf ../$build_version/db_proxy_server/

    cp msfs/build/msfs ../$build_version/msfs/
    cp msfs/build/msfs.conf ../$build_version/msfs/

    cp slog/log4cxx.properties ../$build_version/lib/
    cp slog/build/libslog.so  ../$build_version/lib/
    cp -a slog/lib/liblog4cxx.so* ../$build_version/lib/
    cp sync_lib_for_zip.sh ../$build_version/

    cp tools/daeml ../$build_version/
    cp ../run/restart.sh ../$build_version/

    cd ../
    tar zcvf    $build_name $build_version #这个是压缩，tar zxvf是解压

    rm -rf $build_version
}

clean() {
    cd base/build
    make clean
    cd ../../login_server/build
    make clean
    cd ../../route_server/build
    make clean
    cd ../../msg_server/build
    make clean
    cd ../../http_msg_server/build
    make clean
    cd ../../file_server/build
    make clean
    cd ../../db_proxy_server/build
    make clean
}

case $1 in
    clean)
        echo "clean all build..."
        clean
        ;;
    version)
        if [ $# != 2 ]; then
            echo $#
            exit
        fi
        
        echo $2 
        echo "build..."
        build $2
        ;;
    *)
    ;;
esac
