
cmake_minimum_required(VERSION 3.14)
project(rafa_json)

set(CMAKE_CXX_STANDARD 17)

add_executable(rafa_json
        src/json.hpp
        src/jsonException.hpp
        src/jsonValue.hpp
        src/parse.hpp
        src/json.cpp
        src/jsonValue.cpp
        src/parse.cpp json_test/test.cpp json_test/checkJson.cpp)

target_link_libraries(rafa_json /usr/local/lib/libgtest.a)

add_library (rafajson STATIC src/json.cpp src/jsonValue.cpp src/parse.cpp)

