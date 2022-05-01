#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <pico/stdlib.h>

void keyboard_init();
void keyboard_task();

bool is_something_pressed();
uint8_t* keyboard_get_keys_pressed();

#endif
