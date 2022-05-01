
add_library(keyboard keyboard.c)

target_link_libraries(
	keyboard PUBLIC
	pico_stdlib
	pico_bootrom
)

