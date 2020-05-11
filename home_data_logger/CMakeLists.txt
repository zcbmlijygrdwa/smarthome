cmake_minimum_required(VERSION 3.10)

project(home_data_logger)

include_directories(
include
)

add_subdirectory(serial)

add_executable(main
src/main.cpp
)

target_link_libraries(main PUBLIC
serial
)

add_executable(read_sensor_main
src/read_sensor_main.cpp
)

target_link_libraries(read_sensor_main PUBLIC
serial
)
