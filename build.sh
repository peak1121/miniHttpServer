#!/bin/sh

#开启命令显示
set -x

SOURCE_DIR=`pwd`

#若BUILD_DIR为空则赋值-./build
BUILD_DIR=${BUILD_DIR:-./build}

CXX=${CXX:-g++}

#常见json符号链接方法调试编译过程
ln -sf $BUILD_DIR/compile_commands.json

#创建编译目录并进入编译目录执行cmake 设置编译选项 开启之后生成的json文件包含所以编译单元执行的指令

mkdir -p $BUILD_DIR \
  && cd $BUILD_DIR \
  && cmake \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $*

