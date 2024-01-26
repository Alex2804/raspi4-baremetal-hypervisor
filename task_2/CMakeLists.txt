cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(task2 C ASM)
set(ELF_FILE_NAME task2.elf)
set(BIN_TARGET_NAME task2.img)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)
set(ASM_SRC
        start.S
        src/exceptions/el2.S
        src/exceptions/exceptions.S)
set(C_SRC
        src/main.c
        src/uart.c
        src/exceptions/el2.c
        src/exceptions/exceptions.c
)

add_executable(${ELF_FILE_NAME} ${C_SRC} ${ASM_SRC})
target_include_directories(${ELF_FILE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_options(${ELF_FILE_NAME} PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options(${ELF_FILE_NAME} PUBLIC -T ${LINKER_SCRIPT} -nostdlib)

add_custom_target(${BIN_TARGET_NAME}
        ALL DEPENDS ${ELF_FILE_NAME}
        BYPRODUCTS kernel8img
        COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE_NAME} kernel8.img
)
