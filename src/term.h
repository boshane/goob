#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

enum TERM_KEYS {
	KEY_NULL = 0,
	CTRL_B = 2,
	CTRL_D = 4,
	CTRL_F = 6,
	CTRL_N = 14,
	CTRL_P = 16,
	CTRL_Q = 17,
	ESC = 27
};

void term_init();
int get_cursor_position(int fdi, int fdo, int *row, int *col);

#endif
