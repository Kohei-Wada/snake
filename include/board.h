#ifndef BOARD_H
#define BOARD_H

typedef struct snake snake_t;
typedef struct board board_t;

char **board_get_array_cpy(board_t *b);
char **board_get_array(board_t *b);

int board_get_wid(board_t *b);
int board_get_hgt(board_t *b);

void board_plot_snake(board_t *b, snake_t *s);
void board_set_food(board_t *b);
void board_set_foods(board_t *b,int nfoods);

void board_clear(board_t *b);

int board_init(board_t **b, int wid, int hgt);
void board_free(board_t *b);

#endif
