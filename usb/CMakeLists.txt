add_library(usb)

target_sources(usb PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/usb.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

target_link_libraries(
	usb PUBLIC
	pico_stdlib
	tinyusb_board
	tinyusb_device
)

target_include_directories(usb PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
