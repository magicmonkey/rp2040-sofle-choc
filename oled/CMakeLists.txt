
add_library(oled INTERFACE)

target_sources(oled INTERFACE oled.cpp)

add_subdirectory(pico-ssd1306)

target_link_libraries(
	oled INTERFACE
	pico_stdlib
	hardware_i2c
	pico_ssd1306
)

