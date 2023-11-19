#ifndef command_h_INCLUDED
#define command_h_INCLUDED

#include <sys/ioctl.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

typedef struct {
	int row;
	int col;
} cursor;

typedef union {
	const void *vp;
} parameter;

typedef struct {
        void (*exec)();
        const char* desc;
} command;

enum KEYCOMMANDS {
        FORWARD_CHAR,
        BACKWARD_CHAR,
        FORWARD_WORD,
        BACKWARD_WORD
};

void forward_char(int* cur); 
void backward_char(); 
void forward_word(int *col);
void backward_word();
void next_line(); 
void previous_line(); 
void terminal_set(struct termios *new);
int get_cursor_position(int fdi, int fdo, int *row, int *col);

#endif // command_h_INCLUDED
