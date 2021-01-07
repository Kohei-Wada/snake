#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <assert.h>
#include <termios.h>
#include <time.h>
#include <string.h>

#include "snake.h"
#include "utils.h"


int  **stage  = NULL;
int  **_stage = NULL;

int stage_x, stage_y; //length of stage x and y.

int init_x, init_y;   //start point

int foods     = 10;
int die       = 0;

enum {EASY, NORMAL, HARD};
int mode      = NORMAL;
int got_foods = 0;




void set_food(int **stage)
{
    stage[rand()%(stage_x-2) + 1][rand()%(stage_y-2) + 1] = FOOD;
}


void set_foods(int **stage, int foods)
{
    srand((unsigned int)time(NULL));
    for(int i = 0; i < foods; ++i)
        set_food(stage);
}


void display(int **stage)
{
int x, y;

    for(y = 0; y < stage_y; ++y){
        for(x = 0; x < stage_x; ++x){
            switch (stage[x][y]) {
                case WALL_H: printf("="); break;
                case WALL_V: printf("|"); break;
                case SNAKE : printf("\e[32m+\e[0m"); break;
                case FOOD  : printf("\e[31m@\e[0m"); break;
                case FIELD : printf(" "); break;
            }
        }
        printf("\n");
    }
    printf("you got %d foods ", got_foods);
    switch (mode) {
    case EASY : printf("[EASY]\n"); break;
    case NORMAL: printf("[NORMAL]\n"); break;
    case HARD : printf("[HARD]\n"); break;

    }

}


void init_stage(int **stage, int stage_x,int stage_y)
{
    int x, y;

    for (x = 0; x < stage_x; ++x) {
        stage[x] = (int *)malloc(sizeof(int) * stage_y);
        if (!stage[x]) goto fail;
        for (y = 0; y < stage_y; ++y) stage[x][y] = FIELD;
    }

    for(int y = 0; y < stage_y; ++y)
        stage[0][y] = stage[stage_x-1][y] = WALL_V;

    for(int x = 0; x < stage_x; ++x)
        stage[x][0] = stage[x][stage_y-1] = WALL_H;

    return;


  fail:
    perror("malloc");
    exit(1);
}



void next_snake_v(Snake *snake)
{
    if(kbhit()){
        char ch = getch();
        switch (ch) {
        case 'a':
            if(snake->vx != 1){
                snake->vx = -1;
                snake->vy = 0;
            }
            break;

        case 'f':
            if(snake->vx != -1){
                snake->vx = 1;
                snake->vy = 0;
            }
            break;

        case 'e':
            if(snake->vy != 1){
                snake->vx = 0;
                snake->vy = -1;
            }
            break;

        case 'd':
            if(snake->vy != -1){
                snake->vx = 0;
                snake->vy = 1;
            }
            break;

        case 'q': die = 1; break;

        }
    }
}


void add_head(Snake *snake, int x, int y)
{
Node *node;

    node = malloc(sizeof(Node));
    assert(node != NULL);

    node -> x = x;
    node -> y = y;
    snake -> head -> prev = node;
    node -> next  = snake -> head;
    snake -> head = node;
}


void next_snake(Snake *snake)
{
    Node *current = snake->head;
    int x = current->x + snake->vx;
    int y = current->y + snake->vy;

    switch (stage[x][y]) {

    case WALL_H:
    case WALL_V:
        if (mode == EASY) break;

    case SNAKE:
        die = 1;
        break;

    case FOOD:
        _stage[x][y] = FIELD;
        set_food(_stage);
        add_head(snake, x, y);
        ++got_foods;
        break;

    case FIELD:

        current = snake -> tail;
        while(current -> prev != NULL){
            current -> x = current -> prev -> x;
            current -> y = current -> prev -> y;
            current      = current -> prev;
        }

        snake -> head -> x = x;
        snake -> head -> y = y;
        break;
    }
}


void stage_cpy(int **stage, int **_stage)
{
    int x;
    for (x = 0; x < stage_x; ++x) {
        memcpy(stage[x], _stage[x], sizeof(int) * stage_y);
    }
}


void set_stage(Snake *snake)
{
    Node *current;

    next_snake_v(snake);
    next_snake(snake);
    stage_cpy(stage, _stage);

    current = snake->head;

    while(current != NULL){
        stage[current -> x][current -> y] = SNAKE;
        current = current -> next;
    }
}


void init_snake(Snake *snake)
{
    Node *node;

    node = malloc(sizeof(Node));
    assert(node != NULL);

    node -> x = init_x;
    node -> y = init_y;
    node -> next = NULL;
    node -> prev = NULL;

    snake -> head = node;
    snake -> tail = node;
    snake ->vx    = 1;
}


void usage(void)
{
    printf("[Usage] snake: snake [-E | -h | -f number| -H ]\n");
    printf("   snake game.\n");
    printf("\n");
    printf("[Options]\n");
    printf("   -E    Easy mode.\n");
    printf("   -H    Hard mode.\n");
    printf("   -h    Show this help.\n");
    printf("   -f    If you specify this flag,\n");
    printf("         you can set the number of foods to be set first.\n");
    printf("\n");
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
    Snake *snake;
    struct winsize size;
    int time = 100000;
    int opt;


    while ((opt = getopt(argc, argv, "hEHf:")) != -1) {
        switch (opt) {
        case 'h':
            usage();

        case 'f':
            foods = atoi(optarg);
            break;

        case 'E':
            if (mode != NORMAL) break;

            mode = EASY;
            break;

        case 'H':
            if (mode != NORMAL) break;

            mode = HARD;
            time = 70000;
            break;

        default:
            usage();
        }
    }


    if (ioctl(1, TIOCGWINSZ, &size) == -1)
        return 1;

    stage_x = size.ws_col;
    stage_y = size.ws_row - 2;

    init_x = stage_x / 2;
    init_y = stage_y / 2;


    snake = malloc(sizeof(Snake));
    if (!snake) return 1;

    stage = malloc(sizeof(int *)*stage_x);
    if (!stage) return 1;

    _stage = malloc(sizeof(int *)*stage_x);
    if (!_stage) return 1;


    init_snake(snake);
    init_stage(stage, stage_x, stage_y);
    init_stage(_stage, stage_x, stage_y);

    set_foods(_stage, foods);

    open_termios();

    while (1) {
        system("clear");

        set_stage(snake);
        if (die) break;

        display(stage);
        usleep(time);
    }

    close_termios();

    printf("game over\n");
    printf("you got %d foods\n", got_foods);

    return 0;
}
