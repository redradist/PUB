################################
# GTest
################################
add_subdirectory (../googletest pub/tests/googletest)
enable_testing()
include_directories(${PUB_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Get all test files recursivly
file(GLOB_RECURSE PUB_TEST_SOURCE_FILES pub/*.cpp)

################################
# Unit Tests
################################
# Add test cpp file
add_executable(${PROJECT_NAME}_tests ${PUB_TEST_SOURCE_FILES})
# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main ${PROJECT_NAME})
# Run tests
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${PROJECT_NAME}_tests)