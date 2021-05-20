#ifndef _GETCH_H_
#define _GETCH_H_

void open_termios(void);
void close_termios(void);
int kbhit(void);
char getch(void);


#endif
