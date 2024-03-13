cmake_minimum_required(VERSION 3.5.0)

project(task5 C ASM)

create_baremetal_target(
        task5.elf
        task5.img
        include/
        src/main.c
        src/start.S
        "${CMAKE_CURRENT_SOURCE_DIR}/link.ld"
        rp4bh_utils
)
