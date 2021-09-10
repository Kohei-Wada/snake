#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "ui.h"
#include "snake.h"

typedef struct player {
	game_t *game;
	snake_t *snake;
	ui_t *ui;
	char *key_buf;
	int wid;
	int hgt;
} player_t;


game_t *player_get_game(player_t *p)
{
	return p->game;
}


void player_set_game(player_t *p, game_t *g)
{
	p->game = g;
}


snake_t *player_get_snake(player_t *p)
{
	return p->snake;
}


void player_set_snake(player_t *p, snake_t *s)
{
	p->snake = s;
}


static stype_t random_type()
{
	return random() % 25;
}


int player_init(player_t **p, game_t *g)
{
	*p = malloc(sizeof(player_t));
	player_set_game(*p, g);

	(*p)->key_buf = malloc(sizeof(char));

	int wid = game_get_stage_wid(g);
	int hgt = game_get_stage_hgt(g);

	snake_init(&(*p)->snake, wid/2, hgt/2, random_type());
	ui_init(&(*p)->ui, g, *p);
	return 0;
}


void player_set_key(player_t *p, char key)
{
	*p->key_buf = key;
}


char player_get_key(player_t *p)
{
	return *p->key_buf;
}


void player_free(player_t *p)
{
	free(p->key_buf);
	snake_free(p->snake);
	ui_free(p->ui);
	free(p);
}


int player_update(player_t *p)
{
	ui_update(p->ui);
	return 0;
}
