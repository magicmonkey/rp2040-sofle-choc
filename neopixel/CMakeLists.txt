add_library(neopixel INTERFACE)

target_sources(neopixel INTERFACE neopixel.c)

target_link_libraries(
	neopixel INTERFACE
	pico_stdlib
	hardware_pio
)

