#include "keyboard/keyboard.h"
#include "oled/oled.h"
//#include "usb/usb.h"
//#include "neopixel/neopixel.h"

int main(int argc, char *argv[]) {
	init_keyboard();
	init_oled();
	//init_usb();
	//init_neopixel();

	/*
	buttonsPressed prev;

	for (int i = 0; i < 30; i++) {
		setPixel(i, 0x05050500);
	}
	refreshPixels();

	while (1) {
		tud_task();
		curr = scan();
		if (curr != prev) {
			buttonsChanged(curr, prev);
		}
		prev = curr;
		//setNeopixel(m, 0x11001100);
		hid_task();
	}
	*/
}

