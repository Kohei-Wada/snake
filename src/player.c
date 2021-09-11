#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "ui.h"
#include "snake.h"
#include "board.h"

typedef struct player {
	game_t *game;
	snake_t *snake;
	ui_t *ui;
	char *key_buf;
	int wid;
	int hgt;
} player_t;


void player_set_key(player_t *p, char key)
{
	*p->key_buf = key;
}


char player_get_key(player_t *p)
{
	return *p->key_buf;
}


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


/*TODO make board object*/
int player_update(player_t *p)
{
	game_t *g = player_get_game(p);
	snake_t *s = player_get_snake(p);
	board_t *b = game_get_board(g);

	board_plot_snake(b, s);

	ui_update(p->ui);

	int vx = snake_get_vx(s);
	int vy = snake_get_vy(s);

	int tmpx = snake_get_pos_x(s, 0) + vx;
	int tmpy = snake_get_pos_y(s, 0) + vy;

	//check if snake is dead
	if (!game_get_pause(g)) {
		char **stage = board_get_array(b);
		char **cpy   = board_get_array_cpy(b);
		
		switch (stage[tmpx][tmpy]) {
		case FIELD : 
			snake_update(s);
			break;

		case FOOD: 
			cpy[tmpx][tmpy] = FIELD;
			snake_add(s, tmpx, tmpy);
			board_set_food(b);
			break;
		
		default:
			//dead
			game_set_active(g, 0);
			break;
		}
	}

	//update snake v
	switch (player_get_key(p)) {
	case 'q' : game_set_active(g, 0); break;
	case 'p' : game_set_pause(g, !game_get_pause(g)); break;

	case 'a' : 
		if (vx != 1) 
			snake_set_v(s, -1, 0); 
		break;
	case 'f' : 
		if (vx != -1)
			snake_set_v(s, 1 , 0); 
		break;
	case 'e' : 
		if (vy != 1)
			snake_set_v(s, 0, -1); 
		break; 
	case 'd' : 
		if (vy != -1)
			snake_set_v(s, 0, 1); 
		break;
	}

	//clear key buffer
	player_set_key(p, 0);

	return 0;
}


int player_init(player_t **p, game_t *g)
{
	*p = malloc(sizeof(player_t));
	player_set_game(*p, g);

	(*p)->key_buf = malloc(sizeof(char));

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


