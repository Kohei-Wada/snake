#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "ui.h"
#include "snake.h"
#include "board.h"
#include "observer.h"


typedef struct player {
	observer_t o;
	const char *name;
	game_t *game;
	snake_t *snake;
	ui_t *ui;
	char *key_buf;
} player_t;



void player_set_name(player_t *p, const char *name)
{
	p->name = name;
}


const char *player_get_name(player_t *p)
{
	return p->name;
}


void player_set_key(player_t *p, char key)
{
	*p->key_buf = key;
}


char player_get_key(player_t *p)
{
	return *p->key_buf;
}


ui_t *player_get_ui(player_t *p)
{
	return p->ui;
}


game_t *player_get_game(player_t *p)
{
	return p->game;
}


void player_set_game(player_t *p, game_t *g)
{
	p->game = g;
}


board_t *player_get_board(player_t *p)
{
	game_t *g = player_get_game(p);
	return game_get_board(g);
}


snake_t *player_get_snake(player_t *p)
{
	return p->snake;
}


void player_result(player_t *p)
{
	system("clear");
	printf("game over...\n");
	printf("name   : %s\n", player_get_name(p));
	printf("length : %d\n", snake_len(player_get_snake(p)));
}


void player_print_status(player_t *p)
{
	printf("name : %s,  length : %d\n", player_get_name(p), snake_len(player_get_snake(p)));
}



void player_plot_snake(observer_t *o)
{

	player_t *p = (player_t *)o;

	game_t *g = player_get_game(p);
	snake_t *s = player_get_snake(p);
	board_t *b = game_get_board(g);

	board_plot_snake(b, s);
}



int player_update(observer_t *o)
{
	player_t *p = (player_t *)o;

	game_t *g = player_get_game(p);
	snake_t *s = player_get_snake(p);
	board_t *b = game_get_board(g);

	ui_update(player_get_ui(p));

	/*TODO*/
	if (!game_get_pause(g)) {
		if (board_put_snake(b, s)) {
			game_detach_observer(g, o);
			game_set_active(g, 0);
		}
	}

	/*TODO*/
	switch (player_get_key(p)) {
	case 'q' : game_detach_observer(g, o); game_set_active(g, 0);break;
	case 'p' : game_set_pause(g, !game_get_pause(g)); break;
	case 'a' : snake_set_v(s, -1, 0);  break;
	case 'f' : snake_set_v(s, 1 , 0);  break;
	case 'e' : snake_set_v(s, 0, -1); break; 
	case 'd' : snake_set_v(s, 0, 1); break;
	}

	//clear key buffer
	player_set_key(p, 0);
	return 0;
}


int player_init(player_t **p, game_t *g, const char *name)
{
	*p = malloc(sizeof(player_t));

	observer_set_update_function(&(*p)->o, player_update);
	observer_set_plot_function(&(*p)->o, player_plot_snake);

	(*p)->game = g;
	(*p)->name = name;
	(*p)->key_buf = malloc(sizeof(char));

	board_t *b = game_get_board(g);

	int wid = board_get_wid(b);
	int hgt = board_get_hgt(b);

	snake_init(&(*p)->snake, wid/2, hgt/2);
	ui_init(&(*p)->ui, *p, b);
	return 0;
}


void player_free(player_t *p)
{
	free(p->key_buf);
	snake_free(p->snake);
	ui_free(p->ui);
	free(p);
}
