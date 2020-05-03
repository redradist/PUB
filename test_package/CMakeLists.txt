cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_LIST_DIR}/../src)

add_executable(example example.cpp)
target_link_libraries(example ${CONAN_LIBS})
