cmake_minimum_required(VERSION 3.5.0)

project(host_uart_writer_task6 C ASM)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} CppLinuxSerial::CppLinuxSerial)
