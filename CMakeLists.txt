cmake_minimum_required(VERSION 3.7)
project(pub CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror=return-type -Werror=return-local-addr")

add_subdirectory(src)
add_subdirectory(tests)
