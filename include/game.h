#ifndef _GAME_H_
#define _GAME_H_

#include <pthread.h>
#include "snake.h"
#include "list.h"

enum {FIELD, WALL_H, WALL_V, SNAKE, FOOD};

typedef struct game {
	char *buf;
	int active;
	char **stage;
	char **stage_cpy;
	int stage_wid;
	int stage_hgt;
	snake_t *snake;
	pthread_mutex_t mutex;
} game_t;


int game_init(game_t **g, int wid, int hgt);
void game_free(game_t *g);
void game_loop(game_t *g);
void game_key_add(game_t *g, char key);
char game_current_key(game_t *g);
void game_stage_size(game_t *g, int *wid, int *hgt);
void game_set_snake(game_t *g, snake_t *s);
char **game_get_stage(game_t *g);

#endif
