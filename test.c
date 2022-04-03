#include <pico/bootrom.h>
int main(int argc, char *argv[]) {
	reset_usb_boot(0, 0);
	return 0;
}
