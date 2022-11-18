cmake_minimum_required(VERSION 3.13)
project(battery_manager)

add_library(bm_lib SHARED src/BatteryManager.c)

target_include_directories(bm_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} bm_lib)