cmake_minimum_required(VERSION 3.14)
project(rafa_json)

include_directories(src)
include_directories(test)

add_executable(rafa_json
        src/parser.cpp
        src/rafajson.hpp
        src/json.hpp
        src/json.cpp
        test/test.cpp
        test/test.hpp src/jsonValue.hpp src/jsonValue.cpp src/jsonException.hpp src/parse.hpp)

# 使用c++17标准
add_definitions(-std=c++17)

