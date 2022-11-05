cmake_minimum_required(VERSION "3.15")
project(battery_manager)

add_library(b_m SHARED
	src/BatteryManager.c
)

target_include_directories(b_m PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
	src/main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME}
	b_m
)

install(TARGETS b_m
	DESTINATION ~/bin/automotive/lib
)

install(TARGETS ${PROJECT_NAME}
	DESTINATION ~/bin/automotive/bin
)