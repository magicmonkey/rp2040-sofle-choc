#include <pico/bootrom.h>
#include "ws2812.pio.h"
#include <time.h>
#include <stdio.h>
#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/shapeRenderer/ShapeRenderer.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "hardware/i2c.h"

#include "bsp/board.h"
#include "tusb.h"

typedef struct machine {
	PIO pio;
	uint sm;
} machine;

typedef uint32_t buttonsPressed;

uint32_t pixels[30];
int keyToLed[29] = {
        28,21,20,11,10, 0,
        27,22,19,12, 9, 1,
        26,23,18,13, 8, 2,
        25,24,17,14, 7, 3,
              16,15, 6, 5, 4};

machine neoPixMachine;

machine initNeopixel() {

	//static const uint led_pin = 25;
	static const uint led_pin = 0;

	machine m;

	// Choose PIO instance (0 or 1)
	m.pio = pio0;

	// Get first free state machine in PIO 0
	m.sm = pio_claim_unused_sm(m.pio, true);

	// Add PIO program to PIO instruction memory. SDK will find location and
	// return with the memory offset of the program.
	uint offset = pio_add_program(m.pio, &ws2812_program);

	// Initialize the program using the helper function in our .pio file
	ws2812_program_init(m.pio, m.sm, offset, led_pin, 800000, false);

	// Start running our PIO program in the state machine
	pio_sm_set_enabled(m.pio, m.sm, true);

	return m;
}

void setNeopixel(machine m, uint32_t val) {
	pio_sm_put_blocking(m.pio, m.sm, val);
}

void reset() {
	reset_usb_boot(0, 0);
}

void initScan() {
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

void refreshPixels() {
	for (int i=0; i<29; i++) {
		setNeopixel(neoPixMachine, pixels[i]);
	}
}

void setPixel(int pixNum, uint32_t col) {
	if (pixNum < 29) {
		pixels[keyToLed[pixNum]] = col;
	}
}

void pressed(int btnNum) {

	uint8_t itf = 0;
	char msg[100];
	sprintf(msg, "%d pressed\n", btnNum);
	tud_cdc_n_write(itf, msg, strlen(msg));

	if (btnNum == 0) {
		reset();
	}

	uint8_t scan[6] = {4,0,0,0,0,0}; // We can send up to 6 scan codes at once
	if (tud_hid_ready()) {
		sprintf(msg, "Sending key press\n", btnNum);
		tud_cdc_n_write(itf, msg, strlen(msg));
		tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, scan);
		setPixel(btnNum, 0x33000000);
	} else {
		sprintf(msg, "Not sending key press\n", btnNum);
		tud_cdc_n_write(itf, msg, strlen(msg));
		setPixel(btnNum, 0x00330000);
	}
	refreshPixels();
	tud_cdc_n_write_flush(itf);
}

void unpressed(int btnNum) {
	printf("%d unpressed\n", btnNum);
	setPixel(btnNum, 0x05050500);
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

void initI2C() {
	// Set the special function of GPIO pins 2 and 3 to be I2C1
	gpio_set_function(2, GPIO_FUNC_I2C);
	gpio_set_function(3, GPIO_FUNC_I2C);
	gpio_pull_up(2);
	gpio_pull_up(3);
	i2c_init(i2c1, 1000*1000);
}

void initOled() {
	pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(i2c1, 0x3C, pico_ssd1306::Size::W128xH32);

	sleep_ms(250); // Give the display a little time to initialise
	//pico_ssd1306::drawLine(&display, 0, 0, 128, 32);
	//pico_ssd1306::drawLine(&display, 0, 32, 128, 0);
	pico_ssd1306::drawText(
		&display,
		font_5x8,
		"Hello",
		0, 25,
		pico_ssd1306::WriteMode::ADD,
		pico_ssd1306::Rotation::deg270
	);
	display.sendBuffer();
}

int main(int argc, char *argv[]) {
	initScan();
	initI2C();
	initOled();
	tusb_init();

	neoPixMachine = initNeopixel();

	buttonsPressed curr;
	buttonsPressed prev;

	for (int i = 0; i < 30; i++) {
		setPixel(i, 0x05050500);
	}
	refreshPixels();

	while (1) {
		curr = scan();
		if (curr != prev) {
			buttonsChanged(curr, prev);
		}
		prev = curr;
		//setNeopixel(m, 0x11001100);
		tud_task();
	}
}

