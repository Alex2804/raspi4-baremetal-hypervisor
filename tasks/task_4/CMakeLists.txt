cmake_minimum_required(VERSION 3.5.0)

project(task4 C ASM)

set(C_AND_ASM_SRC kernel/src/main.c kernel/src/start.S)

create_baremetal_target(
        "${PROJECT_NAME}.elf"
        "${PROJECT_NAME}.img"
        kernel/include
        "${C_AND_ASM_SRC}"
        "${CMAKE_CURRENT_SOURCE_DIR}/kernel/link.ld"
        rp4bh_utils
)

add_executable("${PROJECT_NAME}_program.elf" program/src/main.S program/src/main.c program/src/math.c)
target_compile_options("${PROJECT_NAME}_program.elf" PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options("${PROJECT_NAME}_program.elf" PUBLIC -T "${CMAKE_CURRENT_SOURCE_DIR}/program/link.ld" -nostdlib)

add_custom_target("${PROJECT_NAME}_program.img"
        ALL DEPENDS "${PROJECT_NAME}_program.elf"
        BYPRODUCTS "${PROJECT_NAME}_programimg"
        COMMAND ${CMAKE_OBJCOPY} -O binary "${PROJECT_NAME}_program.elf" "${PROJECT_NAME}_program.img"
)

add_custom_target("${PROJECT_NAME}_program.h"
        ALL DEPENDS "${PROJECT_NAME}_program.img"
        COMMAND mkdir -p "kernel/include"
        COMMAND xxd -i -n "program" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_program.img" "${CMAKE_CURRENT_SOURCE_DIR}/kernel/include/${PROJECT_NAME}_program.h"
)

add_dependencies("${PROJECT_NAME}.elf" "${PROJECT_NAME}_program.h")
