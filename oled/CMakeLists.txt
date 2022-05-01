
add_library(oled oled.cpp)

add_subdirectory(pico-ssd1306)

target_link_libraries(
	oled PUBLIC
	pico_stdlib
	hardware_i2c
	pico_ssd1306
)

