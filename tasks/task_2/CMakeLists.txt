cmake_minimum_required(VERSION 3.5.0)

project(task2 C ASM)

set(C_AND_ASM_SRC src/main.c src/start.S)

create_baremetal_target(
        "${PROJECT_NAME}.elf"
        "${PROJECT_NAME}.img"
        ""
        "${C_AND_ASM_SRC}"
        "${CMAKE_CURRENT_SOURCE_DIR}/link.ld"
        rp4bh_utils
)
