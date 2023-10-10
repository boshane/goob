#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>

#define MAX_LEN 5 

enum cursor_movement {
    FORWARD_CHAR,
    BACKWARD_CHAR,
    FORWARD_LINE,
    BACKWARD_LINE,
    END_OF_FILE
};

typedef enum cursor_movement cursor_movement_t;

typedef struct node 
{
  struct node* left;
  struct node* right;
  struct node* par;
  char* str;
  int len;
} node;

typedef struct line {
  struct node** root;
  struct line* next;
  int leafcol;
  int linecol;
} line;

typedef struct buffer {
  struct line* firstline;
  struct node* current_leaf;
  struct line* current_line;
  int length;
  bool dirty;
} buffer;

extern struct buffer buf;

void rope_print(node* rope);
void rope_destroy(node* rope);
void buffer_print();
void buffer_append_string(const char* string);
void make_node(node* parent, node** cur, const char* string, int l, int r);
void buffer_create();
void buffer_destroy();
cursor_movement_t cursor_char_forward();
node* get_first_leaf(node* node);
node* get_last_leaf(node* node);

#endif
