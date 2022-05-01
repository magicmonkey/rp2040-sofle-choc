cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(kbd C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)

add_executable(kbd main.c)

add_subdirectory(keyboard)
add_subdirectory(oled)

#target_sources(kbd PUBLIC
#        main.cpp
#)

# Make sure TinyUSB can find tusb_config.h
#target_include_directories(kbd PUBLIC
#        ${CMAKE_CURRENT_LIST_DIR}/usb)

target_link_libraries(
	kbd PUBLIC
	keyboard
	oled
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(kbd)

