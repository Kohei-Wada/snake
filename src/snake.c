#include <stdlib.h>
#include <stdio.h>

#include "snake.h"

typedef struct snake {
	list_t *l;
	int len;
	int vx;
	int vy;
	stype_t type;
} snake_t;


pos_t *snake_get_pos(snake_t *s, int index)
{
	return list_get(s->l, index);
}


int snake_get_pos_x(snake_t *s, int index)
{
	pos_t *p = list_get(s->l, index);
	return p->x;
}


int snake_get_pos_y(snake_t *s, int index)
{
	pos_t *p = list_get(s->l, index);
	return p->y;
}


int snake_len(snake_t *s)
{
	return s->len;
}


void snake_set_len(snake_t *s, int l)
{
	s->len = l;
}


int snake_get_vx(snake_t *s)
{
	return s->vx;
}


void snake_set_vx(snake_t *s, int v)
{
	s->vx = v;
}


int snake_get_vy(snake_t *s)
{
	return s->vy;
}


void snake_set_vy(snake_t *s, int v)
{
	s->vy = v;
}


void snake_set_v(snake_t *s, int vx, int vy)
{
	s->vx = vx;
	s->vy = vy;
}


const char *stype[] = {
	"\e[30mo\e[0m",
	"\e[31mo\e[0m",
	"\e[32mo\e[0m",
	"\e[33mo\e[0m",
	"\e[34mo\e[0m",
	"\e[35mo\e[0m",
	"\e[36mo\e[0m",
	"\e[37mo\e[0m",

	"\e[30m*\e[0m",
	"\e[31m*\e[0m",
	"\e[32m*\e[0m",
	"\e[33m*\e[0m",
	"\e[34m*\e[0m",
	"\e[35m*\e[0m",
	"\e[36m*\e[0m",
	"\e[37m*\e[0m",

	"\e[30m+\e[0m",
	"\e[31m+\e[0m",
	"\e[32m+\e[0m",
	"\e[33m+\e[0m",
	"\e[34m+\e[0m",
	"\e[35m+\e[0m",
	"\e[36m+\e[0m",
	"\e[37m+\e[0m",
};


const char *snake_get_type(snake_t *s)
{
	stype_t type = s->type;

	if (type == RAINBOW)
		return stype[random()%23];
	else 
		return stype[type];
}

void snake_set_type(snake_t *s, stype_t t)
{
	s->type = t;
}


void snake_add(snake_t *s, int x, int y)
{
	pos_t *p = malloc(sizeof(pos_t));
	
	p->x = x; p->y = y;
	list_add_head(s->l, p);
	++s->len;
}


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


int snake_init(snake_t **s, int x, int y, stype_t type) 
{
	*s = malloc(sizeof(snake_t));
	if (!(*s)) 
		goto error0;

	snake_set_len(*s, 0);
	snake_set_vx(*s, 1);
	snake_set_vy(*s, 0);
	snake_set_type(*s, type);

	if (list_init(&(*s)->l)) 
		goto error1;

	snake_add(*s, x, y);
	return 0;


  error1:
	free(*s);

  error0:
	return 1;

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



