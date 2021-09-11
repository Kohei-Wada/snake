#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "ui.h"
#include "snake.h"
#include "board.h"


typedef struct player {
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


/*TODO*/
static stype_t random_type()
{
	return random() % 25;
}


/*TODO*/
int player_update(player_t *p)
{
	game_t *g = player_get_game(p);
	snake_t *s = player_get_snake(p);
	board_t *b = game_get_board(g);
	ui_t *ui = player_get_ui(p);

	/*warning*/
	board_plot_snake(b, s);
	ui_update(ui);

	if (!game_get_pause(g)) 
		if (board_put_snake(b, s))
			game_detach_player(g, p);


	switch (player_get_key(p)) {
	case 'q' : game_detach_player(g, p); break;
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
	player_set_game(*p, g);

	(*p)->key_buf = malloc(sizeof(char));
	(*p)->name = name;

	board_t *b = game_get_board(g);
	int wid = board_get_wid(b);
	int hgt = board_get_hgt(b);

	snake_init(&(*p)->snake, wid/2, hgt/2, random_type());
	ui_init(&(*p)->ui, g, *p);
	return 0;
}


void player_free(player_t *p)
{
	free(p->key_buf);
	snake_free(p->snake);
	ui_free(p->ui);
	free(p);
}


