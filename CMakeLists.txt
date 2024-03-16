cmake_minimum_required(VERSION 3.14.0)

# external project before project definition to compile with the default toolchain (should be for host)
include(ExternalProject)
ExternalProject_Add(host_uart_writer
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/host_uart_writer
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/host_uart_writer
        INSTALL_COMMAND ""
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(raspi4-baremetal-hypervisor LANGUAGES C ASM)

add_subdirectory(tasks)
