#ifndef GETCH_H
#define GETCH_H

void open_termios(void);
void close_termios(void);
int kbhit(void);
char getch(void);


#endif
