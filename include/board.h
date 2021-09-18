#ifndef BOARD_H
#define BOARD_H

enum elem_type {FIELD = 0, WALL_H, WALL_V, FOOD, SNAKE, SNAKE_HEAD};
typedef enum elem_type etype_t;
typedef struct snake snake_t;
typedef struct board board_t;


int board_put_snake(board_t *b, snake_t *s);
int board_put_enemy(board_t *b, snake_t *s);

int board_get_wid(board_t *b);
int board_get_hgt(board_t *b);

void board_plot_snake(board_t *b, snake_t *s);
void board_set_food(board_t *b);
void board_set_foods(board_t *b,int nfoods);

void board_clear(board_t *b);

int board_init(board_t **b);
void board_free(board_t *b);

const char *board_get_elem_shape(board_t *b, int x, int y);

#endif
