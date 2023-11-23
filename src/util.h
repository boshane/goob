#ifndef util_h_INCLUDED
#define util_h_INCLUDED

#include "../soap/buffer.h"

#define NODE_H 1
#define NODE_W 3
#define SPACING 5
#define CORNER '.'
#define VERTICAL '|'
#define HORIZONTAL '-'

#include <termios.h>

#define CLEAR_SCREEN() (printf("\x1b[H\x1b[J\n"))
#define BG_COLOR(c) (printf("\x1b[48;5;%dm", c))
#define RESET_COLOR() (printf("\x1b[0m"))
#define CURSOR_ROW_COL(i, j) (printf("\x1b[%d;%dH",i,j))
#define CURSOR_ROW(i) (printf("\x1b[H\x1b[%dB",i))
#define CURSOR_COL(j) (printf("\x1b[H\x1b[%dC",j))

void menubar_display(int crow, int ccol);
void pretty_print_rope(node* root);

#endif // util_h_INCLUDED
