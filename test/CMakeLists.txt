cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
message("GTest_INCLUDE_DIRS = ${GTest_INCLUDE_DIRS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Wextra -pedantic-errors -Wno-unused-function -Wno-unused-result -fprofile-arcs -ftest-coverage")

add_library(commonLibrary SHARED ../src/BatteryManager.c)
target_include_directories(commonLibrary PUBLIC ../include)
add_executable(mainApp ../src/main.c)
target_link_libraries(mainApp commonLibrary)

add_executable(unitTestRunner testRunner.cpp)
target_link_libraries(unitTestRunner commonLibrary ${GTEST_LIBRARIES} pthread)