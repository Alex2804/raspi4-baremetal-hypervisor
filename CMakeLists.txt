cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(raspi4-baremetal-hypervisor)

add_subdirectory(task_0)
add_subdirectory(task_1)
add_subdirectory(task_2)
