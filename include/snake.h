#ifndef _SNAKE_H_
#define _SNAKE_H_


typedef struct game game_t;
typedef struct snake snake_t;

typedef struct position {
	int x, y;
} pos_t;

int snake_init(snake_t **s, game_t *g);
void snake_free(snake_t *s);
void snake_add(snake_t *s, int x, int y);
void snake_set_v(snake_t *s, int vx, int vy);
void snake_get_v(snake_t *s, int *vx, int *vy);

void snake_update(snake_t *s);
void snake_test(snake_t *s);
void snake_display(snake_t *s);

int snake_len(snake_t *s);
pos_t *snake_get_pos(snake_t *s, int index);


#endif
