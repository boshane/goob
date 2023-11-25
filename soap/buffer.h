#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <sys/ioctl.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
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
	uint8_t len;
} node;

typedef struct line {
	struct node **root;
	struct line *next;
	uint8_t leafcol;
	uint16_t linecol;
} line;

typedef struct buffer {
	struct line *firstline;
	struct node *current_leaf;
	struct line *current_line;
    char* filename;
    uint8_t pnth;
    char pending[256];
	uint16_t length;
    uint16_t tcol; /* target column */
	bool dirty;
} buffer;

void rope_destroy(node *rope);
void buffer_destroy(buffer **buf);
void buffer_append_string(buffer **buf, char *string);
void buffer_init(buffer **buf, char *str);
int leaf_of_nthchar(node *root, int n, node** dest);
void get_nth_char(node *root, int n, char **c);
int rope_length(node *root);
node * get_first_leaf(node *n);
node * get_next_leaf(node *cur);
void get_last_leaf(node *root, node **last);
node * get_root_node(node *leaf);
void make_node(node *parent, node **cur, const char *string, int l, int r);


#endif
