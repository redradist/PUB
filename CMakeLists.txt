cmake_minimum_required(VERSION 3.7)
project(PUB CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror=return-type -Werror=return-local-addr")

set(SOURCE_FILES
        main.cpp
        src/PackBuffer.hpp
        src/UnpackBuffer.hpp
        src/StackPackBuffer.hpp
        src/HeapPackBuffer.hpp
        tests/HeapPackBufferTests.cpp
        tests/StackPackBufferTests.cpp
        tests/UnpackBufferTests.cpp)

################################
# GTest
################################
add_subdirectory (googletest)
enable_testing()
include_directories(./)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} gtest gtest_main)

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${PROJECT_NAME})
