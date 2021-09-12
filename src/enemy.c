#include <stdlib.h>

#include "enemy.h"
#include "observer.h"
#include "game.h"
#include "snake.h"
#include "board.h"


typedef struct enemy {
	observer_t o;
	const char *name;
	game_t *game;
	snake_t *snake;
} enemy_t;



void enemy_set_name(enemy_t *e, const char *name)
{
	e->name = name;
}


const char *enemy_get_name(enemy_t *e)
{
	return e->name;
}


game_t *enemy_get_game(enemy_t *e)
{
	return e->game;
}


void enemy_set_game(enemy_t *e, game_t *g)
{
	e->game = g;
}



board_t *enemy_get_board(enemy_t *e)
{
	game_t *g = enemy_get_game(e);
	return game_get_board(g);
}


snake_t *enemy_get_snake(enemy_t *e)
{
	return e->snake;
}


void enemy_plot_snake(observer_t *o)
{
	enemy_t *e = (enemy_t *)o;
	game_t *g = enemy_get_game(e);
	snake_t *s = enemy_get_snake(e);
	board_t *b = game_get_board(g);
	board_plot_snake(b, s);
}


int enemy_update(observer_t *o)
{

	enemy_t *e = (enemy_t *)o;
	game_t *g = enemy_get_game(e);
	snake_t *s = enemy_get_snake(e);
	board_t *b = game_get_board(g);

	if (!game_get_pause(g)) 
		if (board_put_enemy(b, s))
			game_detach_observer(g, o);

	return 0;
}



int enemy_init(enemy_t **e, game_t *g, const char *name)
{
	*e = malloc(sizeof(enemy_t));
	enemy_set_game(*e, g);

	observer_set_update_function(&(*e)->o, enemy_update);
	observer_set_plot_function(&(*e)->o, enemy_plot_snake);

	(*e)->name = name;
	board_t *b = game_get_board(g);

	int wid = board_get_wid(b);
	int hgt = board_get_hgt(b);

	snake_init(&(*e)->snake, wid/3, hgt/3, NORMALBLUE);
	return 0;
}


void enemy_free(enemy_t *e)
{
	snake_free(e->snake);
	free(e);
}


