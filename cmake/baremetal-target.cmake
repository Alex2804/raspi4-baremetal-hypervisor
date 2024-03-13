
function(create_baremetal_target ELF_FILE_NAME BIN_TARGET_NAME INCLUDE_DIRECTORIES C_SRC ASM_SRC LINKER_SCRIPT LINK_LIBRARIES)
    add_executable(${ELF_FILE_NAME} ${C_SRC} ${ASM_SRC})
    target_include_directories(${ELF_FILE_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
    target_compile_options(${ELF_FILE_NAME} PUBLIC -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles)
    target_link_options(${ELF_FILE_NAME} PUBLIC -T ${LINKER_SCRIPT} -nostdlib)
    target_link_libraries(${ELF_FILE_NAME} PUBLIC ${LINK_LIBRARIES})

    add_custom_target(${BIN_TARGET_NAME}
            ALL DEPENDS ${ELF_FILE_NAME}
            BYPRODUCTS kernel8img
            COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE_NAME} kernel8.img
    )
endfunction()
