cmake_minimum_required(VERSION 3.5.0)

project(task0 C ASM)
set(ELF_FILE_NAME task0.elf)
set(BIN_TARGET_NAME task0.img)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)
set(ASM_SRC src/start.S)
set(C_SRC src/main.c)

add_executable(${ELF_FILE_NAME} ${C_SRC} ${ASM_SRC})
target_include_directories(${ELF_FILE_NAME} PUBLIC include/)
target_compile_options(${ELF_FILE_NAME} PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options(${ELF_FILE_NAME} PUBLIC -T ${LINKER_SCRIPT} -nostdlib)
target_link_libraries(${ELF_FILE_NAME} PUBLIC rp4bh_utils)

add_custom_target(${BIN_TARGET_NAME}
        ALL DEPENDS ${ELF_FILE_NAME}
        BYPRODUCTS kernel8img
        COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE_NAME} kernel8.img
)
