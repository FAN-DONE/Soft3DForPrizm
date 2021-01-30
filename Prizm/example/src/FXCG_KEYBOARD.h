#ifndef FXCG_KEYBOARD_H
#define FXCG_KEYBOARD_H

#include <fxcg/keyboard.h>


static inline int fxcg_key_down(int basic_keycode);

static inline int fxcg_key_down(int basic_keycode) {
	const volatile unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
	int row, col, word, bit;
	row = basic_keycode % 10;
	col = basic_keycode / 10 - 1;
	word = row >> 1;
	bit = col + ((row & 1) << 3);
	return (0 != (keyboard_register[word] & 1 << bit));
}

#endif
