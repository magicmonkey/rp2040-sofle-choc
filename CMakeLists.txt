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

add_subdirectory(oled)
add_subdirectory(usb)
add_subdirectory(neopixel)
add_subdirectory(keyboard)

add_executable(kbd)

target_sources(kbd PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(kbd PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/usb)

target_link_libraries(
	kbd PUBLIC
	pico_stdlib
	#pico_stdio_usb
	pico_bootrom
	tinyusb_board
	tinyusb_device
	hardware_pio
	pico_ssd1306
	hardware_i2c
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(kbd)
