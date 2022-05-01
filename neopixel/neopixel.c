#include "neopixel.h"
#include "ws2812.pio.h"
#include "../keyboard/keyboard.h"

#define NUM_PIXELS 29

#include <hardware/pio.h>

typedef struct machine {
	PIO pio;
	uint sm;
} machine;

int keyToLed[NUM_PIXELS] = {
        28,21,20,11,10, 0,
        27,22,19,12, 9, 1,
        26,23,18,13, 8, 2,
        25,24,17,14, 7, 3,
              16,15, 6, 5, 4};

// Prototypes which are not externally exposed
void setNeopixel(machine m, uint32_t val);
machine initNeopixelStateMachine();
//////

machine neoPixMachine;
uint32_t pixels[NUM_PIXELS];

void init_neopixel() {
	neoPixMachine = initNeopixelStateMachine();
}

void setAllPixelsTo(uint32_t val) {
	for (int i=0; i<NUM_PIXELS; i++) {
		setPixel(i, val);
	}
}

void setPixel(int pixNum, uint32_t col) {
	if (pixNum < NUM_PIXELS) {
		pixels[keyToLed[pixNum]] = col;
	}
}

void refreshPixels() {
	for (int i=0; i<NUM_PIXELS; i++) {
		setNeopixel(neoPixMachine, pixels[i]);
	}
}

void setNeopixel(machine m, uint32_t val) {
	pio_sm_put_blocking(m.pio, m.sm, val);
}

machine initNeopixelStateMachine() {

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

