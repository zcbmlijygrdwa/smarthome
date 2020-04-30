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
