cmake_minimum_required(VERSION 3.5.0)

project(task1 C ASM)

create_baremetal_target(
        task1.elf
        task1.img
        ""
        src/main.c
        src/start.S
        ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
        rp4bh_utils
)
