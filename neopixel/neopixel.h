#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <pico/stdio.h>
void init_neopixel();
void setAllPixelsTo(uint32_t val);
void setPixel(int pixNum, uint32_t col);
void refreshPixels();

#endif
