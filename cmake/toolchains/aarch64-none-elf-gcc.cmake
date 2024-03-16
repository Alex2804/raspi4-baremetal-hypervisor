# check if the path to the cross compiler is defined
if(NOT DEFINED AARCH64_NONE_ELF_GCC_PATH)
    message(FATAL_ERROR "Path to aarch64-none-elf-gcc in variable AARCH64_NONE_ELF_GCC_PATH is required")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# specify cross compilers and tools
SET(CMAKE_C_COMPILER ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-gcc)
SET(CMAKE_CXX_COMPILER ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-g++)
set(CMAKE_ASM_COMPILER  ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-gcc)
set(CMAKE_AR ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-ar)
set(CMAKE_OBJCOPY ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-objcopy)
set(CMAKE_OBJDUMP ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-objdump)
set(SIZE ${AARCH64_NONE_ELF_GCC_PATH}/aarch64-none-elf-size)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  ${AARCH64_NONE_ELF_GCC_PATH} CACHE PATH "" FORCE)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE STRING "" FORCE)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE STRING "" FORCE)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE STRING "" FORCE)
