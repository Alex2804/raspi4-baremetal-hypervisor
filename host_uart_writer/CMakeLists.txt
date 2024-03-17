cmake_minimum_required(VERSION 3.5.0)

project(host_uart_writer)

add_subdirectory(libs/CppLinuxSerial)

add_executable(host_uart_writer main.cpp)
target_link_libraries(host_uart_writer CppLinuxSerial::CppLinuxSerial)
