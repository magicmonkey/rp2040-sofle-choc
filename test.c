#include <pico/stdio_usb.h>
int main(int argc, char *argv[]) {
	stdio_usb_init();
	while (1);
	return 0;
}
