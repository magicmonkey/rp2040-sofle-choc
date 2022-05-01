#include "keyboard.h"

typedef uint32_t buttonsPressed;
buttonsPressed curr;
uint8_t keys[6];

uint32_t pixels[30];
int keyToLed[29] = {
        28,21,20,11,10, 0,
        27,22,19,12, 9, 1,
        26,23,18,13, 8, 2,
        25,24,17,14, 7, 3,
              16,15, 6, 5, 4};

uint8_t keyToHid[30] = {
	HID_KEY_A,   HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5,
	HID_KEY_TAB, HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T,
	HID_KEY_A,   HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G,
	HID_KEY_A,   HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B,
	HID_KEY_A,   HID_KEY_A, HID_KEY_A, HID_KEY_A, HID_KEY_A, HID_KEY_A,
};

void reset() {
	reset_usb_boot(0, 0);
}

void init_scan() {
	// Rows are outputs
	gpio_init(5);  gpio_set_dir(5,  true);
	gpio_init(6);  gpio_set_dir(6,  true);
	gpio_init(7);  gpio_set_dir(7,  true);
	gpio_init(8);  gpio_set_dir(8,  true);
	gpio_init(9);  gpio_set_dir(9,  true);
	// Cols are inputs with pull-ups
	gpio_init(21); gpio_set_dir(21, false); gpio_pull_up(21);
	gpio_init(23); gpio_set_dir(23, false); gpio_pull_up(23);
	gpio_init(20); gpio_set_dir(20, false); gpio_pull_up(20);
	gpio_init(22); gpio_set_dir(22, false); gpio_pull_up(22);
	gpio_init(26); gpio_set_dir(26, false); gpio_pull_up(26);
	gpio_init(27); gpio_set_dir(27, false); gpio_pull_up(27);
}

uint32_t scan() {
	uint32_t allVals;
	uint32_t retval = 0;

	gpio_put(5, false);
	gpio_put(6, true);
	gpio_put(7, true);
	gpio_put(8, true);
	gpio_put(9, true);
	sleep_ms(1);
	allVals = gpio_get_all();
	retval |= ((allVals & 1<<21) >> 16); // Pos 5
	retval |= ((allVals & 1<<23) >> 19); // Pos 4
	retval |= ((allVals & 1<<20) >> 17); // Pos 3
	retval |= ((allVals & 1<<22) >> 20); // Pos 2
	retval |= ((allVals & 1<<26) >> 25); // Pos 1
	retval |= ((allVals & 1<<27) >> 27); // Pos 0

	gpio_put(5, true);
	gpio_put(6, false);
	gpio_put(7, true);
	gpio_put(8, true);
	gpio_put(9, true);
	sleep_ms(1);
	allVals = gpio_get_all();
	retval |= ((allVals & 1<<21) >> 10); // Pos 5
	retval |= ((allVals & 1<<23) >> 13); // Pos 4
	retval |= ((allVals & 1<<20) >> 11); // Pos 3
	retval |= ((allVals & 1<<22) >> 14); // Pos 2
	retval |= ((allVals & 1<<26) >> 19); // Pos 1
	retval |= ((allVals & 1<<27) >> 21); // Pos 0

	gpio_put(5, true);
	gpio_put(6, true);
	gpio_put(7, false);
	gpio_put(8, true);
	gpio_put(9, true);
	sleep_ms(1);
	allVals = gpio_get_all();
	retval |= ((allVals & 1<<21) >> 4); // Pos 5
	retval |= ((allVals & 1<<23) >> 7); // Pos 4
	retval |= ((allVals & 1<<20) >> 5); // Pos 3
	retval |= ((allVals & 1<<22) >> 8); // Pos 2
	retval |= ((allVals & 1<<26) >> 13); // Pos 1
	retval |= ((allVals & 1<<27) >> 15); // Pos 0

	gpio_put(5, true);
	gpio_put(6, true);
	gpio_put(7, true);
	gpio_put(8, false);
	gpio_put(9, true);
	sleep_ms(1);
	allVals = gpio_get_all();
	retval |= ((allVals & 1<<21) << 2); // Pos 5
	retval |= ((allVals & 1<<23) >> 1); // Pos 4
	retval |= ((allVals & 1<<20) << 1); // Pos 3
	retval |= ((allVals & 1<<22) >> 2); // Pos 2
	retval |= ((allVals & 1<<26) >> 7); // Pos 1
	retval |= ((allVals & 1<<27) >> 9); // Pos 0

	gpio_put(5, true);
	gpio_put(6, true);
	gpio_put(7, true);
	gpio_put(8, true);
	gpio_put(9, false);
	sleep_ms(1);
	allVals = gpio_get_all();
	retval |= ((allVals & 1<<21) << 8); // Pos 5
	retval |= ((allVals & 1<<23) << 5); // Pos 4
	retval |= ((allVals & 1<<20) << 7); // Pos 3
	retval |= ((allVals & 1<<22) << 4); // Pos 2
	retval |= ((allVals & 1<<26) >> 1); // Pos 1
	retval |= ((allVals & 1<<27) >> 3); // Pos 0

	retval = ~(retval | 0xc0000000);

	//printf("%d - %u\n", allVals, retval);

	return retval;
}

void pressed(int btnNum) {

	setPixel(btnNum, 0x55000000);

	if (btnNum == 0) {
		reset();
	}

	for (int i=0; i<6; i++) {
		if (keys[i] == 0) {
			keys[i] = btnNum;
		}
	}

	refreshPixels();
}

void unpressed(int btnNum) {
	setPixel(btnNum, 0x05050500);
	for (int i=0; i<6; i++) {
		if (keys[i] == btnNum) {
			keys[i] = 0;
		}
	}
	refreshPixels();
}

inline bool isPressed(buttonsPressed b, int btnNum) {
	if (b & 1<<btnNum) {
		return true;
	}
	return false;
}

void buttonsChanged(buttonsPressed curr, buttonsPressed prev) {
	for (int i=0; i<30; i++) {
		if (isPressed(curr, i)) {
			if (!isPressed(prev, i)) {
				pressed(i);
			}
		} else {
			if (isPressed(prev, i)) {
				unpressed(i);
			}
		}
	}
}

void translateKeysToHid(uint8_t *srcKeys, uint8_t *dstKeys) {
	for (int i=0; i<6; i++) {
		dstKeys[i] = keyToHid[srcKeys[i]];
	}
	return;
}

void init_keyboard() {
	init_scan();
}

