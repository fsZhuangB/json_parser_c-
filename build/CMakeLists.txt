# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (rafa_json)

# 指定生成目标
add_executable(rafa /Users/fszhuangb/Documents/rafa_json/test.cpp)

# 使用c++11标准
add_definitions(-std=c++11)
