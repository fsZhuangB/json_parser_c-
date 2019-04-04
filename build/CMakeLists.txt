# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (rafa_json)

# 手动添加源文件
set(SOURCES ../src/parser.cpp ../src/rafajson.hpp ../test/test.cpp ../test/test.hpp)

# 指定生成目标
add_executable(Rafa ${SOURCES})

# 使用c++11标准
add_definitions(-std=c++17)
