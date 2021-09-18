#include <stdlib.h>

#include "list.h"
#include "snake.h"


typedef struct position {
	int x, y;
} pos_t;


typedef struct snake {
	list_t *l;
	int len;
	int vx;
	int vy;
	stype_t stype;
	scolor_t color;
	int dead;
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


void snake_set_v(snake_t *s, int tvx, int tvy)
{
	int vx = snake_get_vx(s);
	int vy = snake_get_vy(s);

	//can not move backword or diagonally.
	if (tvx * tvy != 0 || tvx * vx < 0 || tvy * vy < 0)
		return;

	snake_set_vx(s, tvx);
	snake_set_vy(s, tvy);
}


void snake_set_color(snake_t *s, scolor_t c)
{
	s->color = c;
}


scolor_t snake_get_color(snake_t *s)
{
	return s->color;
}


void snake_set_stype(snake_t *s, stype_t t)
{
	s->stype = t;
}


stype_t snake_get_stype(snake_t *s)
{ 
	return s->stype;
}


const char *head_shape[] = {
	"\e[31m>\e[0m",
	"\e[32m>\e[0m",
	"\e[33m>\e[0m",
	"\e[34m>\e[0m",
	"\e[35m>\e[0m",
	"\e[36m>\e[0m",
	"\e[37m>\e[0m",

	"\e[31m<\e[0m",
	"\e[32m<\e[0m",
	"\e[33m<\e[0m",
	"\e[34m<\e[0m",
	"\e[35m<\e[0m",
	"\e[36m<\e[0m",
	"\e[37m<\e[0m",

	"\e[31m^\e[0m",
	"\e[32m^\e[0m",
	"\e[33m^\e[0m",
	"\e[34m^\e[0m",
	"\e[35m^\e[0m",
	"\e[36m^\e[0m",
	"\e[37m^\e[0m",

	"\e[31mv\e[0m",
	"\e[32mv\e[0m",
	"\e[33mv\e[0m",
	"\e[34mv\e[0m",
	"\e[35mv\e[0m",
	"\e[36mv\e[0m",
	"\e[37mv\e[0m",
};


/*TODO*/
const char *snake_get_head_shape(snake_t *s)
{
	int vx = snake_get_vx(s), vy = snake_get_vy(s);
	scolor_t c = snake_get_color(s);
	if (vx == 1)
		return head_shape[c];
	else if(vx == -1)
		return head_shape[c + 7];
	else if (vy == -1)
		return head_shape[c + 7*2];
	else 
		return  head_shape[c + 7*3];
}

const char *snake_shape[] = {
	//"\e[30mo\e[0m",
	"\e[31mo\e[0m",
	"\e[32mo\e[0m",
	"\e[33mo\e[0m",
	"\e[34mo\e[0m",
	"\e[35mo\e[0m",
	"\e[36mo\e[0m",
	"\e[37mo\e[0m",

	//"\e[30m*\e[0m",
	"\e[31m*\e[0m",
	"\e[32m*\e[0m",
	"\e[33m*\e[0m",
	"\e[34m*\e[0m",
	"\e[35m*\e[0m",
	"\e[36m*\e[0m",
	"\e[37m*\e[0m",

	//"\e[30m+\e[0m",
	"\e[31m+\e[0m",
	"\e[32m+\e[0m",
	"\e[33m+\e[0m",
	"\e[34m+\e[0m",
	"\e[35m+\e[0m",
	"\e[36m+\e[0m",
	"\e[37m+\e[0m",
};


/*WARNING*/
const char *snake_get_shape(snake_t *s)
{
	return snake_shape[snake_get_stype(s) * 7 + snake_get_color(s)];
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


static stype_t random_snake_type() 
{
	return random() % 3;
}

static scolor_t random_snake_color()
{
	return random() % 7;
}


int snake_init(snake_t **s, int x, int y) 
{
	*s = malloc(sizeof(snake_t));
	if (!(*s)) 
		goto error0;

	if (list_init(&(*s)->l)) 
		goto error1;

	snake_set_len(*s, 0);
	snake_set_vx(*s, 1);
	snake_set_vy(*s, 0);

	snake_add(*s, x, y);

	snake_set_stype(*s, random_snake_type());
	snake_set_color(*s, random_snake_color());

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



