#include <termios.h>
#include <unistd.h>


static struct termios old, current;
static int read_char = -1;



void open_termios(void)
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    current.c_lflag &= ~ECHO;
    current.c_lflag &= ~ISIG;
    current.c_cc[VMIN]  = 0;
    current.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &current);
}



void close_termios(void)
{
    tcsetattr(0, TCSANOW, &old);
}


int kbhit(void)
{
char ch;
int nread;

    if(read_char != -1){
        return 1;
    }

    current.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &current);
    nread = read(0, &ch, 1);
    current.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &current);

    if(nread == 1){
        read_char = ch;
        return 1;
    }

    return 0;

}



char getch(void)
{
char ch;
    if(read_char != -1){
        ch = read_char;
        read_char = -1;
        return ch;
    }

    read(0, &ch, 1);
    return ch;
}
