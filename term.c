#include "term.h"


void 
terminal_set (struct termios* new) {
  tcgetattr(STDIN_FILENO, new);
  new->c_lflag &= ~(ICANON|ECHO|IEXTEN|ISIG);
  new->c_iflag &= ~(BRKINT|ICRNL|ISTRIP|INPCK|IXON);
  new->c_oflag &= ~(OPOST);
  new->c_cc[VMIN] = 1;
  new->c_cc[VTIME] = 0;                     
  tcsetattr(STDIN_FILENO, TCSAFLUSH, new);
}

int
get_cursor_position(int fdi, int fdo, struct cursor* cur) {
    int ttyfd = open("/dev/tty", O_RDWR);
    char buf[32];
    unsigned int i = 0;

    write(fdo, "\x1b[6n\n", 5);

    while (i < sizeof(buf)-1) {
      if (read(fdi, buf+i, 1) != 1) break;
      if (buf[i] == 'R') break;
      i++;
    }
    buf[i] = '\0';
    
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(buf+2,"%d;%d", cur->row, cur->col) != 2) return -1;

    return 0;
}
