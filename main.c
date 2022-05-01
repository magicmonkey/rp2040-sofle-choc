#include "keyboard/keyboard.h"
#include <stdio.h>
#include "oled/oled.h"
#include "neopixel/neopixel.h"
#include "usb/usb.h"

int main(int argc, char *argv[]) {
	sleep_ms(5000);
	printf("1\n");
	keyboard_init();
	printf("2\n");
	oled_init();
	printf("3\n");
	neopixel_init();
	printf("4\n");
	usb_init();
	printf("5\n");

	setAllPixelsTo(0x05050500);
	printf("6\n");
	refreshPixels();
	printf("7\n");

	while (1) {
		//setNeopixel(m, 0x11001100);
		keyboard_task();
		usb_task();
	}
}

