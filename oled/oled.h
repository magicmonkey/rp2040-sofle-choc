#ifndef OLED_H
#define OLED_H

extern "C" void init_oled();

#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/shapeRenderer/ShapeRenderer.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#endif
