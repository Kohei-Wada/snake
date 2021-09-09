#ifndef SNAKE_H
#define SNAKE_H

#include "list.h"

typedef struct game game_t;
typedef struct snake snake_t;


typedef enum { 
	NORMALBLACK=0,
	NORMALRED,
	NORMALGREEN,
	NORMALYELLOW,
	NORMALBLUE,
	NORMALMAGENTA,
	NORMALCYAAN,
	NORMALWHITE,

	STARBLACK,
	STARRED,
	STARGREEN,
	STARYELLOW,
	STARBLUE,
	STARMAGENTA,
	STARCYAAN,
	STARWHITE,

	PLUSBLACK,
	PLUSRED,
	PLUSGREEN,
	PLUSYELLOW,
	PLUSBLUE,
	PLUSMAGENTA,
	PLUSCYAAN,
	PLUSWHITE=23,

	RAINBOW,
} stype_t;


typedef struct position {
	int x, y;
} pos_t;

int snake_get_vx(snake_t *s);
void snake_set_vx(snake_t *s, int v);
int snake_get_vy(snake_t *s);
void snake_set_vy(snake_t *s, int v);


int snake_init(snake_t **s, game_t *g, int x, int y, stype_t type);
void snake_free(snake_t *s);
void snake_add(snake_t *s, int x, int y);
void snake_set_v(snake_t *s, int vx, int vy);
void snake_get_v(snake_t *s, int *vx, int *vy);
void snake_update(snake_t *s);
int snake_len(snake_t *s);

pos_t *snake_get_pos(snake_t *s, int index);
int snake_get_pos_x(snake_t *s, int index);
int snake_get_pos_y(snake_t *s, int index);

const char *snake_get_type(snake_t *s);

#endif
