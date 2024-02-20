cmake_minimum_required(VERSION 3.5.0)

include(../cmake/toolchain-aarch64-none-elf-gcc.cmake)

project(task1 C ASM)
set(ELF_FILE_NAME task1.elf)
set(BIN_TARGET_NAME task1.img)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)
set(ASM_SRC
        src/start.S
        src/exceptions/exceptions.S
        src/exceptions/el2.S)
set(C_SRC
        src/main.c
        src/exceptions/exceptions.c
        src/exceptions/el2.c
)

add_executable(${ELF_FILE_NAME} ${C_SRC} ${ASM_SRC})
target_include_directories(${ELF_FILE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_options(${ELF_FILE_NAME} PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options(${ELF_FILE_NAME} PUBLIC -T ${LINKER_SCRIPT} -nostdlib)
target_link_libraries(${ELF_FILE_NAME} rp4bh_utils)

add_custom_target(${BIN_TARGET_NAME}
        ALL DEPENDS ${ELF_FILE_NAME}
        BYPRODUCTS kernel8img
        COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE_NAME} kernel8.img
)
