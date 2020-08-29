#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <termios.h>
#include "snake.h"


#define STAGE_X 100
#define STAGE_Y 50


enum Elements{
    FIELD,
    SNAKE,
    FOOD,
    WALL_1,
    WALL_2
} elem;

int _stage[STAGE_X][STAGE_Y] = {0};
int  stage[STAGE_X][STAGE_Y] = {0};
int die = 0;
int get_foods = 0;



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




void add_head(Snake *snake, int x, int y)
{
Node *node;

    node = (Node *)malloc(sizeof(Node));
    assert(node != NULL);

    node -> x = x;
    node -> y = y;

    snake -> head -> prev = node;
    node -> next = snake -> head;

    snake -> head = node;

}


void init_snake(Snake *snake)
{
Node *node;

    node = (Node *)malloc(sizeof(Node));
    assert(node != NULL);

    node -> x = 50;
    node -> y = 20;
    node -> next = NULL;
    node -> prev = NULL;

    snake -> head = node;
    snake -> tail = node;
}



void set_food()
{
    _stage[rand()%(STAGE_X-2) + 1][rand()%(STAGE_Y-2)+1] = FOOD;
}


void set_foods()
{
int num = 10;
    for(int i = 0; i < num; ++i)
        set_food();
}



void init_stage(Snake *snake)
{
    for(int y = 0; y < STAGE_Y; ++y)
        _stage[0][y] = _stage[STAGE_X-1][y] = WALL_2;

    for(int x = 0; x < STAGE_X; ++x)
        _stage[x][0] = _stage[x][STAGE_Y-1] = WALL_1;

    set_foods();

    init_snake(snake);
}




void next_snake(Snake *snake, int vx, int vy)
{
Node *curret = snake -> head;

    int x = curret -> x + vx;
    int y = curret -> y + vy;

    if(stage[x][y] == FIELD){
        curret = snake -> tail;

        while(curret -> prev != NULL){
            curret -> x = curret -> prev -> x;
            curret -> y = curret -> prev -> y;
            curret      = curret -> prev;
        }
        snake -> head -> x = x;
        snake -> head -> y = y;
    }

    else if(stage[x][y] == FOOD){
        _stage[x][y] = FIELD;
        ++get_foods;
        set_food();
        add_head(snake, x, y);
    }
    else if(stage[x][y] == SNAKE)
        die = 1;
}



void set_stage(Snake *snake)
{
static int vx=1, vy;

    if(kbhit()){
        char ch = getch();
        switch (ch) {
            case 'a':
            {
                if(vx != 1){
                    vx = -1;
                    vy = 0;
                }
                break;
            }
            case 'f':
            {
                if(vx != -1){
                    vx = 1;
                    vy = 0;
                }
                break;
            }
            case 'e':
            {
                if(vy != 1){
                    vx = 0;
                    vy = -1;
                }
                break;
            }
            case 'd':
            {
                if(vy != -1){
                    vx = 0;
                    vy = 1;
                }
                break;
            }
            case 'q': die = 1; break;
        }
    }


    next_snake(snake, vx, vy);
    memcpy(stage, _stage, sizeof(_stage));

    Node *curret;
    curret = snake -> head;

    while(curret != NULL){
        stage[curret -> x][curret -> y] = SNAKE;
        curret = curret -> next;

    }
}



void display(void)
{
    for(int y = 0; y < STAGE_Y; ++y){
        for(int x = 0; x < STAGE_X; ++x){

            switch (stage[x][y]) {
                case WALL_1: printf("="); break;
                case WALL_2: printf("|"); break;
                case SNAKE : printf("+"); break;
                case FOOD  : printf("@"); break;
                default    : printf(" "); break;
            }
        }
        printf("\n");
    }
    printf("you got %d foods\n", get_foods);
}



int main(void)
{

int time = 100000 * 1;
Snake *snake;

    snake = (Snake *)malloc(sizeof(Snake));
    assert(snake != NULL);

    open_termios();

    init_stage(snake);
    while(1){
        system("clear");
        set_stage(snake);
        if(die)
            break;
        display();
        usleep(time);
    }

    close_termios();

    printf("game over\n");
    printf("you got %d foods\n", get_foods);

    return 0;
}
