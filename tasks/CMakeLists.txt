cmake_minimum_required(VERSION 3.14.0)

include(cmake/baremetal-target.cmake)

project(raspi4-baremetal-hypervisor-tasks)

add_subdirectory(libs/utils rp4bh_utils)

add_subdirectory(task_0)
add_subdirectory(task_1)
add_subdirectory(task_2)
add_subdirectory(task_3)
add_subdirectory(task_4)
add_subdirectory(task_5)
add_subdirectory(task_6)
