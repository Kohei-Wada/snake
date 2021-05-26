#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#include "snake.h"
#include "game.h"



static void game_set_food(game_t *g)
{
	char **stage = g->stage_cpy;
	stage[rand() % (g->stage_wid - 2) + 1][rand() % (g->stage_hgt - 2) + 1] = FOOD;
}

static void game_set_foods(game_t *g, int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; ++i)
		game_set_food(g);
}

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


void game_set_snake(game_t *g, snake_t *s)
{
	g->snake = s;
}


int game_init(game_t **g, int wid, int hgt) 
{
	int ret;
	*g = malloc(sizeof(game_t));
	if (!(*g)) {
		perror("malloc"); 
		return 1;
	}

	(*g)->buf       = malloc(sizeof(char));
	(*g)->active    = 1;
	(*g)->stage_wid = wid;
	(*g)->stage_hgt = hgt;
	(*g)->stage     = stage_init(wid, hgt);
	(*g)->stage_cpy = stage_init(wid, hgt);


	if (!(*g)->stage || !(*g)->stage_cpy)
		return 1;

	game_set_foods(*g, 10);

	ret = pthread_mutex_init(&(*g)->mutex, NULL);
	if (ret) {
		perror("mutex_init");
		return 1;
	}

	return ret;
}


void game_stage_size(game_t *g, int *wid, int *hgt)
{
	*wid = g->stage_wid;
	*hgt = g->stage_hgt;
}


static void game_plot_snake(game_t *g)
{
	char **stage = g->stage;
	char **cpy   = g->stage_cpy;
	snake_t *s   = g->snake;

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

	game_plot_snake(g);
	int tmpx, tmpy, vx, vy;
	pos_t *head = snake_get_pos(g->snake, 0);
	snake_get_v(g->snake, &vx, &vy);

	tmpx = head->x + vx; 
	tmpy = head->y + vy;

	//check if snake is dead
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


	//update snake v
	if (*g->buf) {
		switch (*g->buf) {
		case 'q' : 
			g->active = 0; 
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

		//clear key buf
		pthread_mutex_lock(&g->mutex);
		*g->buf = 0;
		pthread_mutex_unlock(&g->mutex);
	}
}

void game_free(game_t *g)
{
	pthread_mutex_destroy(&g->mutex);
	free(g);
}


void game_loop(game_t *g)
{
	while (g->active) {
		game_update(g);
		usleep(70000);
	}
}

char **game_get_stage(game_t *g)
{
	return g->stage;
}


void game_key_add(game_t *g, char key) 
{
	if (*g->buf == 0) {
		pthread_mutex_lock(&g->mutex);
		*g->buf = key;
		pthread_mutex_unlock(&g->mutex);
	}
}



static void save_result(int data)
{
	char file[100];
	char buf[100];

	sprintf(file, "/home/%s/.snake/data", getlogin());

	FILE *f = fopen(file, "a");
	if (!f) return;

	time_t t;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);

	sprintf(buf, "%d/%d/%d/%d/%d/%d,%d\n", tm->tm_year + 1900, tm->tm_mon + 1,
			tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,data);

	fwrite(buf, strlen(buf), 1, f);

	fclose(f);
}




void game_result(game_t *g)
{
	int len = snake_len(g->snake);
	system("clear");

	printf("game over...\n");
	printf("your length is %d\n", len);

	save_result(len);
}



