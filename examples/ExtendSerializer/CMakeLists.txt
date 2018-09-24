cmake_minimum_required(VERSION 3.7)
project(ExtendedSerializer)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

set(SOURCE_FILES    main.cpp
                    SuperPuperClass.cpp
                    SuperPuperClass.hpp)
add_executable(ExtendedSerializer ${SOURCE_FILES})
