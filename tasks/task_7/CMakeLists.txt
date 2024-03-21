cmake_minimum_required(VERSION 3.5.0)

project(task7 C ASM)

set(C_AND_ASM_SRC kernel/src/main.c kernel/src/start.S)

create_baremetal_target(
        "${PROJECT_NAME}.elf"
        "${PROJECT_NAME}.img"
        kernel/include
        "${C_AND_ASM_SRC}"
        "${CMAKE_CURRENT_SOURCE_DIR}/kernel/link.ld"
        rp4bh_utils
)

# valid program
add_executable("valid_${PROJECT_NAME}_program.elf" program/src/main.S program/src/valid_main.c program/src/math.c)
target_compile_options("valid_${PROJECT_NAME}_program.elf" PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options("valid_${PROJECT_NAME}_program.elf" PUBLIC -T "${CMAKE_CURRENT_SOURCE_DIR}/program/link.ld" -nostdlib)

add_custom_target("valid_${PROJECT_NAME}_program.img"
        ALL DEPENDS "valid_${PROJECT_NAME}_program.elf"
        BYPRODUCTS "valid_${PROJECT_NAME}_programimg"
        COMMAND ${CMAKE_OBJCOPY} -O binary "valid_${PROJECT_NAME}_program.elf" "valid_${PROJECT_NAME}_program.img"
)

add_dependencies("${PROJECT_NAME}.elf" "valid_${PROJECT_NAME}_program.img")

# invalid program
add_executable("invalid_${PROJECT_NAME}_program.elf" program/src/main.S program/src/invalid_main.c program/src/math.c)
target_compile_options("invalid_${PROJECT_NAME}_program.elf" PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
target_link_options("invalid_${PROJECT_NAME}_program.elf" PUBLIC -T "${CMAKE_CURRENT_SOURCE_DIR}/program/link.ld" -nostdlib)

add_custom_target("invalid_${PROJECT_NAME}_program.img"
        ALL DEPENDS "invalid_${PROJECT_NAME}_program.elf"
        BYPRODUCTS "invalid_${PROJECT_NAME}_programimg"
        COMMAND ${CMAKE_OBJCOPY} -O binary "invalid_${PROJECT_NAME}_program.elf" "invalid_${PROJECT_NAME}_program.img"
)

add_dependencies("${PROJECT_NAME}.elf" "invalid_${PROJECT_NAME}_program.img")
