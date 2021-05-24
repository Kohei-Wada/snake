#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "list.h"
#include "snake.h"


typedef struct snake {
	list_t *l;
	int len;
	int vx, vy;
} snake_t;


void snake_update(snake_t *s)
{
	pos_t *current, *prev;
	for (int i = s->len - 1; i > 0; --i) {
		current = list_get(s->l, i);
		prev = list_get(s->l , i - 1);
		current->x = prev->x; 
		current->y = prev->y;
	}

	current = list_get(s->l, 0);
	current->x += s->vx;
	current->y += s->vy;
}


void snake_set_v(snake_t *s, int vx, int vy)
{
	s->vx = vx;
	s->vy = vy;
}

void snake_get_v(snake_t *s, int *vx, int *vy)
{
	*vx = s->vx; 
	*vy = s->vy;
}


int snake_init(snake_t **s, game_t *g, int x, int y) 
{
	int ret;

	*s = malloc(sizeof(snake_t));
	if (!(*s)) {
		perror("malloc");
		return 1;
	}

	(*s)->len = 0;
	(*s)->vx = 1;
	(*s)->vy = 0;

	ret = list_init(&(*s)->l);
	if (ret) {
		perror("list_init");
		return 1;
	}

	snake_add(*s, x, y);
	game_set_snake(g, *s);

	return 0;

}


void snake_free(snake_t *s) 
{
	for (int i = 0; i < s->len; ++i) {
		pos_t *p = list_get(s->l, i); 
		free(p);
	}
	list_free(s->l);
	free(s);
}


pos_t *snake_get_pos(snake_t *s, int index)
{
	return list_get(s->l, index);
}


int snake_len(snake_t *s)
{
	return s->len;
}


void snake_add(snake_t *s, int x, int y)
{
	pos_t *p = malloc(sizeof(pos_t));
	
	p->x = x; p->y = y;
	list_add_head(s->l, p);
	++s->len;
}


