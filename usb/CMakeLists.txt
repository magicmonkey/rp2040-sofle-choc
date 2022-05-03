add_library(usb INTERFACE)

target_sources(usb INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/usb.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

target_link_libraries(
	usb INTERFACE
	pico_stdlib
	tinyusb_board
	tinyusb_device
)

target_include_directories(usb INTERFACE
        ${CMAKE_CURRENT_LIST_DIR})
