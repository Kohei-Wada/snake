#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <termios.h>
#include <time.h>
#include "snake.h"
#include "utils.h"

#define STAGE_X 100
#define STAGE_Y 50



int _stage[STAGE_X][STAGE_Y] = {0};
int  stage[STAGE_X][STAGE_Y] = {0};
enum FIELD_ELEMS elem;

int init_x = STAGE_X/2;
int init_y = STAGE_Y/2;
int foods  = 10;
int vx     = 1;
int vy     = 0;
int die    = 0;


int get_foods = 0;
int easy      = 0;




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

    node -> x = init_x;
    node -> y = init_y;
    node -> next = NULL;
    node -> prev = NULL;

    snake -> head = node;
    snake -> tail = node;
}




void set_food()
{
    _stage[rand()%(STAGE_X-2) + 1][rand()%(STAGE_Y-2) + 1] = FOOD;
}




void set_foods()
{
    for(int i = 0; i < foods; ++i)
        set_food();
}



void init_stage(Snake *snake)
{
    srand((unsigned int)time(NULL));
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
    else if(stage[x][y] == WALL_1 || stage[x][y] == WALL_2){
        if(!easy)
            die = 1;
    }
    else if(stage[x][y] == SNAKE)
        die = 1;
}



void get_key()
{

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
}



void set_stage(Snake *snake)
{

    get_key();
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
                case SNAKE : printf("\e[32m+\e[0m"); break;
                case FOOD  : printf("\e[31m@\e[0m"); break;
                case FIELD : printf(" "); break;
            }
        }
        printf("\n");
    }
    printf("you got %d foods\n", get_foods);
}



void usage(void)
{
    printf("[Usage] snake game\n");
    printf("   snake -e -> easy mode\n");
    printf("   snake -h -> show this help\n");
    printf("[Operations]\n");
    printf("   f : right\n");
    printf("   e : up\n");
    printf("   d : down\n");
    printf("   a : left\n");
    printf("   q : quit\n");
    exit(0);
}



int main(int argc, char **argv)
{
int time = 100000 * 1;
Snake *snake;

    if(argc > 1){
        if(strcmp(argv[1], "-h") == 0)
            usage();
        else if(strcmp(argv[1], "-e") == 0)
            easy = 1;
        else
            usage();
    }


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
    free(snake);

    printf("game over\n");
    printf("you got %d foods\n", get_foods);

    return 0;
}
