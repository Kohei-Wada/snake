#ifndef SNAKE_H
#define SNAKE_H

#define N_SNAKE_TYPES (22)

typedef struct snake snake_t;

typedef enum {
	NORMAL=0,
	STAR,
	PLUS,
} stype_t;

typedef enum {
	//BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAAN,
	WHITE,
} scolor_t;


const char *snake_get_shape(snake_t *s);

void snake_set_v(snake_t *s, int vx, int vy);
void snake_get_v(snake_t *s, int *vx, int *vy);

int snake_get_vx(snake_t *s);
void snake_set_vx(snake_t *s, int v);

int snake_get_vy(snake_t *s);
void snake_set_vy(snake_t *s, int v);

int snake_init(snake_t **s, int x, int y);
void snake_free(snake_t *s);

void snake_add(snake_t *s, int x, int y);
void snake_update(snake_t *s);
int snake_len(snake_t *s);

int snake_get_pos_x(snake_t *s, int index);
int snake_get_pos_y(snake_t *s, int index);


#endif
