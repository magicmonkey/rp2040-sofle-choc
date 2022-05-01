
add_library(oled oled.cpp)

target_link_libraries(
	oled PUBLIC
	pico_stdlib
	hardware_i2c
)

