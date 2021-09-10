#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>

#include "getch.h"
#include "game.h"
#include "ui.h"
#include "snake.h"
#include "player.h"


typedef struct ui {
	game_t *g;
	player_t *player;
} ui_t;


static game_t* ui_get_game(ui_t *ui)
{
	return ui->g;
}


void ui_set_game(ui_t *ui, game_t *g)
{
	ui->g = g;
}

void ui_set_player(ui_t *ui, player_t *p)
{
	ui->player = p;
}


player_t *ui_get_player(ui_t *ui)
{
	return  ui->player;
}



static void ui_display(ui_t *ui)
{
	game_t *g = ui_get_game(ui);
	char **stage = game_get_stage(g);
	snake_t *s = game_get_snake(g);

	for (int y = 0; y < game_get_stage_hgt(g); ++y) {

		for (int x = 0; x < game_get_stage_wid(g); ++x) {

			switch (stage[x][y]) {
			case FIELD: 
				printf(" ");
				break;

			case WALL_H: 
				printf("=");
				break;

			case WALL_V: 
				printf("|");
				break;

			case FOOD: 
				printf("\e[31m@\e[0m"); 
				break;

			case SNAKE:
				printf("%s", snake_get_type(s));
				break;
			}
		}
		printf("\n");
	}

	if (game_get_pause(g))
		printf("pause : press 'p' to continue\n");
	else 
		printf("your length is %d\n", snake_len(game_get_snake(g)));
}


void ui_update(ui_t *ui)
{
	player_t *p = ui_get_player(ui);

	system("clear");
	ui_display(ui);

	if (kbhit()) 
		player_set_key(p, getch());
}


int ui_init(ui_t **ui, game_t *g, player_t *p)
{

	*ui = malloc(sizeof(ui_t));
	if (!(*ui)) {
		perror("malloc"); 
		return 1;
	}

	
	ui_set_player((*ui), p);
	ui_set_game((*ui), g);
	open_termios();
	return 0;
}


void ui_free(ui_t *ui) 
{
	close_termios();
	free(ui);
}


