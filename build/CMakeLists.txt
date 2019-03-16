# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (rafa_json)

# 查找当前目录下所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(/Users/fszhuangb/Documents/rafa_json/src DIR_SRCS)

# 指定生成目标
add_executable(Rafa ${DIR_SRCS})

# 使用c++11标准
add_definitions(-std=c++11)
