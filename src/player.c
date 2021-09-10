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


int player_init(player_t **p, game_t *g)
{
	*p = malloc(sizeof(player_t));
	player_set_game(*p, g);

	(*p)->key_buf = malloc(sizeof(char));

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
	ui_free(p->ui);
	free(p->key_buf);
	free(p);
}


int player_update(player_t *p)
{
	ui_update(p->ui);
	return 0;
}
