cmake_minimum_required(VERSION 3.5.0)

project(task6 C ASM)

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
