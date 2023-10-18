#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "term.h"
#include "buffer.h"
#include "util.h"

#define CLEAR_SCREEN    "\x1b[H\x1b[J"

enum KEY_CODES {
  KEY_NULL = 0,
  CTRL_B = 2, 
  CTRL_D = 4, 
  CTRL_F = 6, 
  CTRL_N = 14,
  CTRL_P = 16,
  CTRL_Q = 17,
  ESC = 27
};

const struct command commands[END_OF_FILE] = {
    { FORWARD_CHAR, "Move forward one character", &com_forward_char },
    { BACKWARD_CHAR, "Move backward one character", &com_backward_char },
    { FORWARD_WORD, "Move forward one word", &com_forward_word },
    { BACKWARD_WORD, "Move backward one word", &com_backward_word },
    { PREVIOUS_LINE, "Move to the previous line", &com_previous_line },
    { NEXT_LINE, "Move to the next line", &com_next_line }
};

/* save the termios settings and restore at exit */
static struct termios termios_prev;
/* the buffer for the file */
struct buffer buf;

int
main(int argc, char **argv) {
  struct termios* termios_new;
  termios_new = malloc(sizeof(struct termios));

  /* store the previous termios settings */
  tcgetattr(STDIN_FILENO, &termios_prev);
  /* set the terminal to non-canonical mode */
  terminal_set(termios_new); 
  /* using puts since it flushes the buffer */
  puts(CLEAR_SCREEN);

  buffer_create();
  buffer_append_string("this is 10");
  buffer_append_string("and this is twenty20");
  buffer_append_string("");
  buffer_append_string("fjsdlkfja s;lkfj s;dlkfj ;lkj jsdfa;lfj lsdjf l;kTWO");
  buffer_append_string("two");


  puts(CLEAR_SCREEN);
  buffer_print(buf);
  pretty_print_rope(*buf.firstline->root);
  fflush(stdout);

  int nread;
  char c, seq[3];

    while(1) {
    read(STDIN_FILENO, &c, 1);

    if (c == ESC) {
        read(STDIN_FILENO, seq, 1);

        switch(seq[0]) {
            case 'q':
            	tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_prev);
        		puts(CLEAR_SCREEN);
            	return 0;
            case 'f':
            	/* add a function for moving forward one word in a rope */
            	break;
            case 'b':
            	/* add a function for moving backward one word in a rope */
            	break;
        }
    } 
    else {
        cursor_movement_t input;

        switch(c) {
        case CTRL_F:
            input = cursor_char_forward();
            switch(input) {
                case FORWARD_CHAR:
                    printf("\x1b[C");
                    break;
                case END_OF_FILE:
                    break;
                default:
                    break;
            }
    	}
    }
    fflush(stdout);
  }
  buffer_destroy();
  fflush(stdout);
  return 0;
}
