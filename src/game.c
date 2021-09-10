#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

#include "game.h"
#include "list.h"

typedef struct game {
	char    **stage;
	char    **stage_cpy; 
	int       stage_wid;
	int       stage_hgt;
	int       pause;
	int       active;
	int       nfoods;
	list_t   *players;
	int       nplayers;
} game_t;


/******************************************************************/

static char **stage_init(int wid, int hgt)
{
	char **stage = malloc(sizeof(char *) * wid);

	for (int i = 0; i < wid; ++i) 
		stage[i] = malloc(sizeof(char) * hgt);

	for (int y = 0; y < hgt; ++y) 
		stage[0][y] = stage[wid - 1][y] = WALL_V;

	for (int x = 0; x < wid; ++x)
		stage[x][0] = stage[x][hgt - 1] = WALL_H;

	return stage;
}


static void stage_free(char **stage, int wid)
{
	for (int i = 0; i < wid; ++i)
		free(stage[i]);

	free(stage);
}

/******************************************************************/



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


/*TODO*/
void game_attach_player(game_t *g, player_t *p)
{
	list_add_head(g->players, p);
	g->nplayers++;
}


/*TODO*/
void game_detach_player(game_t *g, player_t *p)
{
	list_delete(g->players, p);
	g->nplayers--;
	return;
}

static char **game_get_stage_cpy(game_t *g)
{
	return g->stage_cpy;
}


char **game_get_stage(game_t *g)
{
	return g->stage;
}


void game_set_stage_wid(game_t *g, int wid)
{
	g->stage_wid = wid;
}


int game_get_stage_wid(game_t *g)
{
	return g->stage_wid;
}


void game_set_stage_hgt(game_t *g, int hgt)
{
	g->stage_hgt = hgt;
}


int game_get_stage_hgt(game_t *g)
{
	return g->stage_hgt;
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


static void game_set_food(game_t *g)
{
	char **stage = g->stage_cpy;
	stage[rand() % (g->stage_wid - 2) + 1][rand() % (g->stage_hgt - 2) + 1] = FOOD;
}


static void game_set_foods(game_t *g)
{
	for (int i = 0; i < game_get_nfoods(g) ; ++i)
		game_set_food(g);
}


static void game_clear_all_snake(game_t *g)
{
	int i, wid = game_get_stage_wid(g), hgt = game_get_stage_hgt(g);
	char **stage = game_get_stage(g), **cpy = game_get_stage_cpy(g);
	for (i = 0; i < wid; ++i) 
		memcpy(stage[i], cpy[i], sizeof(char) * hgt);
}


static void game_plot_snake(game_t *g, snake_t *s)
{
	int x, y;
	char **stage = game_get_stage(g); 
	for (int i = 0; i < snake_len(s); ++i) {
		x = snake_get_pos_x(s, i);
		y = snake_get_pos_y(s, i);
		stage[x][y] = SNAKE;
	}
}


/*TODO fix method name*/
static void update_snake_and_player(game_t *g, player_t *p)
{
	snake_t *s = player_get_snake(p);
	game_plot_snake(g, s);
	player_update(p);

	int vx = snake_get_vx(s);
	int vy = snake_get_vy(s);

	int tmpx = snake_get_pos_x(s, 0) + vx;
	int tmpy = snake_get_pos_y(s, 0) + vy;
	

	//check if snake is dead
	if (!game_get_pause(g)) {
		char **stage = game_get_stage(g);
		char **cpy = game_get_stage_cpy(g);
		
		switch (stage[tmpx][tmpy]) {
		case FIELD : 
			snake_update(s);
			break;

		case FOOD: 
			cpy[tmpx][tmpy] = FIELD;
			snake_add(s, tmpx, tmpy);
			game_set_food(g);
			break;
		
		default:
			//dead
			game_set_active(g, 0);
			break;
		}
	}

	switch (player_get_key(p)) {
	case 'q' : 
		game_set_active(g, 0);
		break;

	case 'p' :
		game_set_pause(g, !game_get_pause(g));
		break;

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
}



/*TODO*/
static void game_update(game_t *g)
{
	game_clear_all_snake(g);

	for (int i = 0; i < game_get_nplayers(g); ++i) {
		player_t *p = game_get_player(g, 0);
		update_snake_and_player(g, p);
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


int game_update_winsize(game_t *g)
{
	struct winsize size;
	if (ioctl(1, TIOCGWINSZ, &size) == -1) 
		return 1;

	game_set_stage_wid(g, size.ws_col); 
	game_set_stage_hgt(g, size.ws_row - 2); 

	return 0;
}



int game_init(game_t **g) 
{
	srand(time(NULL));

	*g = malloc(sizeof(game_t));
	if (!(*g)) 
		return 1;

	if (game_update_winsize(*g))
		return 1;

	int wid = game_get_stage_wid(*g);
	int hgt = game_get_stage_hgt(*g);


	list_init(&(*g)->players);

	game_set_active(*g, 1);
	game_set_pause(*g, 0);
	game_set_nfoods(*g, 10);
	game_set_nplayers(*g, 0);


	(*g)->stage     = stage_init(wid, hgt);
	(*g)->stage_cpy = stage_init(wid, hgt);

	if (!(*g)->stage  || !(*g)->stage_cpy) 
		return 1;

	//after initialized stage, set foods
	game_set_foods(*g);

	return 0;
}


void game_free(game_t *g)
{
	stage_free(g->stage, g->stage_wid);
	stage_free(g->stage_cpy, g->stage_wid);
	list_free(g->players);
	free(g);
}


