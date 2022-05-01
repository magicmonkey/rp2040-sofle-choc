#include "keyboard/keyboard.h"
#include "oled/oled.h"
#include "neopixel/neopixel.h"
#include "usb/usb.h"

int main(int argc, char *argv[]) {
	keyboard_init();
	oled_init();
	neopixel_init();
	usb_init();

	setAllPixelsTo(0x05050500);
	refreshPixels();

	while (1) {
		//tud_task();
		//setNeopixel(m, 0x11001100);
		keyboard_task();
		//hid_task();
	}
}

