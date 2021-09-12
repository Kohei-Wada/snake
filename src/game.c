#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "game.h"
#include "board.h"
#include "list.h"
#include "observer.h"


typedef struct game {
	list_t   *observers;
	board_t  *board;
	int       pause;
	int       nfoods;
	int       nobservers;
} game_t;



static list_t *game_get_observers(game_t *g)
{
	return g->observers;
}


static observer_t *game_get_observer(game_t *g, int i)
{
	list_t *observers = game_get_observers(g);
	return list_get(observers, i);
}



board_t *game_get_board(game_t *g)
{
	return g->board;
}


static int game_get_nobservers(game_t *g)
{
	return g->nobservers;
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


void game_attach_observer(game_t *g, observer_t *o)
{
	list_add_head(g->observers, o);
	g->nobservers++;
}


void game_detach_observer(game_t *g, observer_t *o)
{
	list_delete(g->observers, o);
	g->nobservers--;
}


static void game_update(game_t *g)
{
	board_t *b = game_get_board(g);

	//clear all snakes
	board_clear(b);

	//update each observer
	for (int i = 0; i < game_get_nobservers(g); ++i) {
		observer_t *o = game_get_observer(g, i);
		observer_update(o);
	}
}


void game_loop(game_t *g)
{
	board_t *b = game_get_board(g);
	board_set_foods(b, game_get_nfoods(g));

	while (game_get_nobservers(g) > 0) {
		game_update(g);
		usleep(70000);
	}
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

	list_init(&(*g)->observers);

	board_init(&(*g)->board, wid, hgt);

	game_set_pause(*g, 0);
	(*g)->nobservers = 0;

	/*TODO*/
	game_set_nfoods(*g, 10);


	return 0;
}


void game_free(game_t *g)
{
	list_free(g->observers);
	board_free(g->board);
	free(g);
}


