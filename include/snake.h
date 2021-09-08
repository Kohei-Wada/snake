#ifndef SNAKE_H
#define SNAKE_H

#include "list.h"

typedef struct game game_t;
typedef struct snake snake_t;

typedef struct position {
	int x, y;
} pos_t;

int snake_get_vx(snake_t *s);
void snake_set_vx(snake_t *s, int v);
int snake_get_vy(snake_t *s);
void snake_set_vy(snake_t *s, int v);


int snake_init(snake_t **s, game_t *g, int x, int y, int color);
void snake_free(snake_t *s);
void snake_add(snake_t *s, int x, int y);
void snake_set_v(snake_t *s, int vx, int vy);
void snake_get_v(snake_t *s, int *vx, int *vy);
void snake_update(snake_t *s);
int snake_len(snake_t *s);
pos_t *snake_get_pos(snake_t *s, int index);

int snake_get_color(snake_t *s);

#endif
