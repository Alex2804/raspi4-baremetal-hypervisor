cmake_minimum_required(VERSION 3.5.0)

project(task2 C ASM)

create_baremetal_target(
        task2.elf
        task2.img
        include/
        src/main.c
        src/start.S
        ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
        rp4bh_utils
)
