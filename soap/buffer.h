#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <sys/ioctl.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LEN 5

typedef enum {
        CURSOR_UPDATE,
        LINE_UPDATE, 
        BUFFER_UPDATE,
        NO_UPDATE
} command_result;

typedef command_result command_result_t;

typedef struct node {
	struct node *left;
	struct node *right;
	struct node *par;
	char *str;
	int len;
} node;

typedef struct line {
	struct node **root;
	struct line *next;
	int leafcol;
	int linecol;
} line;

typedef struct buffer {
	struct line *firstline;
	struct node *current_leaf;
	struct line *current_line;
    char* filename;
	int length;
	bool dirty;
} buffer;

void rope_destroy(node *rope);
void rope_print(node *rope);
void buffer_destroy(buffer **buf);
void buffer_display(buffer **buf);
void buffer_append_string(buffer **buf, char *string);
void buffer_init(buffer **buf, char *str);
int leaf_of_nthchar(node *root, int n, node** dest);
void get_nth_char(node *root, int n, char **c);
int rope_length(node *root);
node * get_first_leaf(node *n);
node * get_next_leaf(node *cur, node *prev);
void get_last_leaf(node *root, node **last);
node * get_root_node(node *leaf);
void make_node(node *parent, node **cur, const char *string, int l, int r);

#endif
