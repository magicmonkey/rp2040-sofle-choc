
add_library(keyboard INTERFACE)

target_sources(keyboard INTERFACE keyboard.c)

target_link_libraries(
	keyboard INTERFACE
	pico_stdlib
	pico_bootrom
)

