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
add_subdirectory(neopixel)
add_subdirectory(usb)

target_link_libraries(
	kbd PUBLIC
	pico_stdlib
	#pico_stdio_usb
	keyboard
	oled
	neopixel
	usb
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(kbd)

