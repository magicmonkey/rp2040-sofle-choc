add_library(neopixel neopixel.c)

target_link_libraries(
	neopixel PUBLIC
	pico_stdlib
	hardware_pio
)

