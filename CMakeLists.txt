cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
include(cmake/toolchain-aarch64-none-elf-gcc.cmake)

project(raspi4-baremetal-hypervisor)

add_subdirectory(libs/utils rp4bh_utils)

add_subdirectory(task_0)
add_subdirectory(task_1)
add_subdirectory(task_2)
