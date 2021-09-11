#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

#include "board.h"
#include "game.h"
#include "list.h"


typedef struct game {
	list_t   *players;
	int       nplayers;
	board_t  *board;
	int       pause;
	int       active;
	int       nfoods;
} game_t;


board_t *game_get_board(game_t *g)
{
	return g->board;
}


static void game_set_nplayers(game_t *g, int n)
{
	g->nplayers = n;
}


static int game_get_nplayers(game_t *g)
{
	return g->nplayers;
}


static list_t *game_get_players(game_t *g)
{
	return g->players;
}


static player_t *game_get_player(game_t *g, int i)
{
	list_t *players = game_get_players(g);
	return list_get(players, i);
}


void game_attach_player(game_t *g, player_t *p)
{
	list_add_head(g->players, p);
	g->nplayers++;
}


void game_detach_player(game_t *g, player_t *p)
{
	list_delete(g->players, p);
	g->nplayers--;
}




void game_set_active(game_t *g, int a)
{
	g->active = a;
}


int game_get_active(game_t *g)
{
	return g->active;
}


int game_get_pause(game_t *g)
{
	return g->pause;
}


void game_set_pause(game_t *g, int p)
{
	g->pause = p;
}


int game_get_nfoods(game_t *g)
{
	return g->nfoods;
}


void game_set_nfoods(game_t *g, int n)
{
	g->nfoods = n;
}


/*TODO*/
static void game_update(game_t *g)
{
	board_clear(g->board);

	//update each player
	for (int i = 0; i < game_get_nplayers(g); ++i) {
		player_t *p = game_get_player(g, i);
		player_update(p);
	}
}


void game_loop(game_t *g)
{
	while (game_get_active(g)) {
		game_update(g);
		usleep(70000);
	}
}


/*TODO*/
void game_result(game_t *g)
{
	player_t *p = game_get_player(g, 0);

	system("clear");
	printf("game over...\n");
	printf("your length is %d\n", snake_len(player_get_snake(p)));
}


/*TODO*/
int game_update_winsize(game_t *g, int *wid, int *hgt)
{
	struct winsize size;
	if (ioctl(1, TIOCGWINSZ, &size) == -1) 
		return 1;

	*wid = size.ws_col;
	*hgt = size.ws_row - 2; 

	return 0;
}


int game_init(game_t **g) 
{
	int wid, hgt;
	srand(time(NULL));

	*g = malloc(sizeof(game_t));
	if (!(*g)) 
		return 1;

	if (game_update_winsize(*g, &wid, &hgt))
		return 1;

	game_set_active(*g, 1);
	game_set_pause(*g, 0);
	game_set_nplayers(*g, 0);

	list_init(&(*g)->players);
	board_init(&(*g)->board, wid, hgt);


	//after initialized stage, set foods
	board_set_foods((*g)->board, 10);

	return 0;
}


void game_free(game_t *g)
{
	list_free(g->players);
	board_free(g->board);
	free(g);
}


