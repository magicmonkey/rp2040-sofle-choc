#include "oled.h"

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

