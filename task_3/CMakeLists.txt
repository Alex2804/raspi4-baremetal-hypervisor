cmake_minimum_required(VERSION 3.5.0)

project(task3 C ASM)

create_baremetal_target(
        task3.elf
        task3.img
        include/
        src/main.c
        src/start.S
        ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
        rp4bh_utils
)
