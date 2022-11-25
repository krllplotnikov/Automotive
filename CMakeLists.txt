cmake_minimum_required(VERSION 3.13)
project(battery_manager)

include_directories("${PROJECT_SOURCE_DIR}/include")

#add_library(${PROJECT_NAME}_lib SHARED src/BatteryManager.c)
#target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB C_SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${C_SOURCES})

#target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)