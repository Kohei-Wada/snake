#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "list.h"
#include "ui.h"

typedef struct game game_t;

enum {FIELD, WALL_H, WALL_V, SNAKE, FOOD};


int game_get_stage_wid(game_t *g);
int game_get_stage_hgt(game_t *g);

int game_init(game_t **g, int wid, int hgt);
void game_free(game_t *g);
void game_loop(game_t *g);
void game_set_key(game_t *g, char key);
char game_current_key(game_t *g);
void game_stage_size(game_t *g, int *wid, int *hgt);
void game_set_snake(game_t *g, snake_t *s);
void game_set_ui(game_t *g, ui_t *ui);
char **game_get_stage(game_t *g);
void game_result(game_t *g);
int game_get_pause(game_t *g);
snake_t *game_get_snake(game_t *g);
#endif
