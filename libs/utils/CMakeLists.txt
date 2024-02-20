cmake_minimum_required(VERSION 3.5.0)

project(rp4bh_utils C)

set(C_SRC
        src/uart.c
        src/registers/spsr_elx.c
        src/registers/hcr_el2.c
)

add_library(rp4bh_utils ${C_SRC})
target_include_directories(rp4bh_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
