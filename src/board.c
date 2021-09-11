#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "snake.h"


typedef struct board {
	char **array;
	char **array_cpy;
	int wid;
	int hgt;
} board_t;


/******************************************************************/

static char **array_init(int wid, int hgt)
{
	char **array = malloc(sizeof(char *) * wid);

	for (int i = 0; i < wid; ++i) 
		array[i] = malloc(sizeof(char) * hgt);

	for (int y = 0; y < hgt; ++y) 
		array[0][y] = array[wid - 1][y] = WALL_V;

	for (int x = 0; x < wid; ++x)
		array[x][0] = array[x][hgt - 1] = WALL_H;

	return array;
}


static void array_free(char **array, int wid)
{
	for (int i = 0; i < wid; ++i)
		free(array[i]);

	free(array);
}


/******************************************************************/


char **board_get_array_cpy(board_t *b)
{
	return b->array_cpy;
}


char **board_get_array(board_t *b)
{
	return b->array;
}


int board_get_wid(board_t *b)
{
	return b->wid;
}


int board_get_hgt(board_t *b)
{
	return b->hgt;
}


void board_plot_snake(board_t *b, snake_t *s)
{
	int x, y;
	char **array = board_get_array(b); 
	for (int i = 0; i < snake_len(s); ++i) {
		x = snake_get_pos_x(s, i);
		y = snake_get_pos_y(s, i);
		array[x][y] = SNAKE;
	}
}


void board_set_food(board_t *b)
{
	char **array = b->array_cpy;
	array[rand() % (b->wid - 2) + 1][rand() % (b->hgt - 2) + 1] = FOOD;
}


void board_set_foods(board_t *b,int nfoods)
{
	for (int i = 0; i < nfoods ; ++i)
		board_set_food(b);
}


void board_clear(board_t *b)
{
	int i, wid = board_get_wid(b), hgt = board_get_hgt(b);
	char **array = board_get_array(b), **cpy = board_get_array_cpy(b);

	for (i = 0; i < wid; ++i) 
		memcpy(array[i], cpy[i], sizeof(char) * hgt);
}


int board_put_snake(board_t *b, snake_t *s)
{
	//snake's next head position
	int tmpx = snake_get_pos_x(s, 0) + snake_get_vx(s);
	int tmpy = snake_get_pos_y(s, 0) + snake_get_vy(s);

	char **array = board_get_array(b);
	char **cpy   = board_get_array_cpy(b);
	
	switch (array[tmpx][tmpy]) {
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
		return 1;
	}

	return 0;
}


int board_init(board_t **b, int wid, int hgt)
{
	*b = malloc(sizeof(board_t));
	(*b)->wid = wid;
	(*b)->hgt = hgt;

	(*b)->array     = array_init(wid, hgt);
	(*b)->array_cpy = array_init(wid, hgt);

	return 0;
}


void board_free(board_t *b)
{
	array_free(b->array, b->wid);
	array_free(b->array_cpy, b->wid);
	free(b);
}

