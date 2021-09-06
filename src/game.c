#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "game.h"


typedef struct game {
	char     *key_buf;
	ui_t     *ui;        //ui object
	snake_t  *snake;     //snake object
	char    **stage;
	char    **stage_cpy;
	int       stage_wid;
	int       stage_hgt;
	int       pause;
	int       active;
	int       nfoods;
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


char game_get_key(game_t *g)
{
	return *g->key_buf;
}


void game_set_key(game_t *g, char key) 
{
	*g->key_buf = key;
}


void game_set_ui(game_t *g, ui_t *ui)
{
	g->ui = ui;
}


ui_t *game_get_ui(game_t *g)
{
	return g->ui;
}


snake_t *game_get_snake(game_t *g)
{
	return g->snake;
}


void game_set_snake(game_t *g, snake_t *s)
{
	g->snake = s;
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
	srand(time(NULL));
	for (int i = 0; i < game_get_nfoods(g) ; ++i)
		game_set_food(g);
}


void game_stage_size(game_t *g, int *wid, int *hgt)
{
	*wid = g->stage_wid;
	*hgt = g->stage_hgt;
}


static void game_plot_snake(game_t *g)
{
	char **stage = game_get_stage(g);
	char **cpy   = game_get_stage_cpy(g);
	snake_t *s   = game_get_snake(g);

	for (int i = 0; i < g->stage_wid; ++i) {
		memcpy(stage[i], cpy[i], sizeof(char) * g->stage_hgt);
	}

	for (int i = 0; i < snake_len(s); ++i) {
		pos_t *pos;
		pos = snake_get_pos(s, i);
		stage[pos->x][pos->y] = SNAKE;
	}
}


static void game_update(game_t *g)
{

	snake_t *s = game_get_snake(g);
	ui_t *ui = game_get_ui(g);

	game_plot_snake(g);
	ui_update(ui);

	int vx = snake_get_vx(s);
	int vy = snake_get_vy(s);

	pos_t *head = snake_get_pos(s, 0);
	int tmpx = head->x + vx; 
	int tmpy = head->y + vy;

	//check if snake is dead
	if (!g->pause) {
		switch (g->stage[tmpx][tmpy]) {
		case WALL_H:
		case WALL_V:
		case SNAKE :
			g->active = 0;//dead
			break;

		case FIELD : 
			snake_update(g->snake);
			break;

		case FOOD: 
			g->stage_cpy[tmpx][tmpy] = FIELD;
			snake_add(g->snake, tmpx, tmpy);
			game_set_food(g);
			break;
		}
	}


	switch (game_get_key(g)) {
	case 'q' : 
		game_set_active(g, 0);
		break;

	case 'p' :
		game_set_pause(g, !game_get_pause(g));
		break;

	case 'a' : 
		if (vx != 1) 
			snake_set_v(g->snake, -1, 0); 
		break;

	case 'f' : 
		if (vx != -1)
			snake_set_v(g->snake, 1 , 0); 
		break;

	case 'e' : 
		if (vy != 1)
			snake_set_v(g->snake, 0, -1); 
		break; 

	case 'd' : 
		if (vy != -1)
			snake_set_v(g->snake, 0, 1); 
		break;
	}

	//clear key buffer
	game_set_key(g, 0);
}


void game_loop(game_t *g)
{
	while (game_get_active(g)) {
		game_update(g);
		usleep(70000);
	}
}


void game_result(game_t *g)
{
	int len = snake_len(g->snake);

	system("clear");

	printf("game over...\n");
	printf("your length is %d\n", len);

}


int game_init(game_t **g, int wid, int hgt) 
{

	*g = malloc(sizeof(game_t));
	if (!(*g)) 
		goto error0;

	if (snake_init(&(*g)->snake, *g, wid / 2, hgt / 2)) 
		goto error1;

	(*g)->stage     = stage_init(wid, hgt);
	(*g)->stage_cpy = stage_init(wid, hgt);
	(*g)->key_buf   = malloc(sizeof(char));

	if (!(*g)->stage  || !(*g)->stage_cpy || !(*g)->key_buf) 
		goto error2;

	game_set_active(*g, 1);
	game_set_pause(*g, 0);
	game_set_stage_wid(*g, wid);
	game_set_stage_hgt(*g, hgt);
	game_set_nfoods(*g, 10);
	game_set_foods(*g);


	if (ui_init(&(*g)->ui, *g)) 
		goto error3;

	return 0;



  error3:
	ui_free((*g)->ui);

  error2:
	free((*g)->stage);
	free((*g)->stage_cpy);
	free((*g)->key_buf);

  error1:
	snake_free((*g)->snake);

  error0:
	free(*g);

	return 1;
}


void game_free(game_t *g)
{
	stage_free(g->stage, g->stage_wid);
	stage_free(g->stage_cpy, g->stage_wid);

	free(g->key_buf);
	snake_free(g->snake);
	ui_free(g->ui);

	free(g);
}


