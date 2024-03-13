cmake_minimum_required(VERSION 3.5.0)

project(task4 C ASM)

create_baremetal_target(
        task4.elf
        task4.img
        kernel/include/
        kernel/src/main.c
        kernel/src/start.S
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/link.ld
        rp4bh_utils
)


add_executable(task_4_program.elf program/src/main.S program/src/main.c program/src/math.c)
target_compile_options(task_4_program.elf PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options(task_4_program.elf PUBLIC -T "${CMAKE_CURRENT_SOURCE_DIR}/program/link.ld" -nostdlib)

add_custom_target(task_4_program.img
        ALL DEPENDS task_4_program.elf
        BYPRODUCTS task_4_programimg
        COMMAND ${CMAKE_OBJCOPY} -O binary task_4_program.elf task_4_program.img
)

add_custom_target(task_4_program.h
        ALL DEPENDS task_4_program.img
        COMMAND mkdir -p "${CMAKE_CURRENT_SOURCE_DIR}/kernel/include"
        COMMAND xxd -i -n "program" "${CMAKE_CURRENT_BINARY_DIR}/task_4_program.img" "${CMAKE_CURRENT_SOURCE_DIR}/kernel/include/task_4_program.h"
)

add_dependencies(task4.elf task_4_program.h)
