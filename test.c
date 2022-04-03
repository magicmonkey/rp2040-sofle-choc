#include <pico/stdio_usb.h>
#include "ws2812.pio.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	stdio_usb_init();

	static const uint led_pin = 25;

    // Choose PIO instance (0 or 1)
    PIO pio = pio0;

    // Get first free state machine in PIO 0
    uint sm = pio_claim_unused_sm(pio, true);

    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset = pio_add_program(pio, &ws2812_program);

    // Initialize the program using the helper function in our .pio file
    ws2812_program_init(pio, sm, offset, led_pin, 800000, false);

    // Start running our PIO program in the state machine
    pio_sm_set_enabled(pio, sm, true);

	while (1) {
		printf("Loop\n");
		pio_sm_put_blocking(pio, sm, 0x33000000);
		sleep_ms(700);
		pio_sm_put_blocking(pio, sm, 0x00330000);
		sleep_ms(700);
		pio_sm_put_blocking(pio, sm, 0x00003300);
		sleep_ms(700);
	}
	return 0;
}
