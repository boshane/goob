#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

typedef struct cursor {
  int* row;
  int* col;
} cursor;

void terminal_set (struct termios* new);
int get_cursor_position(int fdi, int fdo, cursor* cur);

#endif // !
