#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>

#include "getch.h"
#include "game.h"
#include "ui.h"


typedef struct ui {
	game_t *g;
	pthread_t handle;
	char **stage;
	int stage_wid;
	int stage_hgt;
} ui_t;


static game_t* ui_get_game(ui_t *ui)
{
	return ui->g;
}


void ui_set_game(ui_t *ui, game_t *g)
{
	ui->g = g;
}


void ui_set_stage(ui_t *ui,  char **s)
{
	ui->stage = s;
}


static void ui_display(ui_t *ui)
{
	game_t *g = ui_get_game(ui);

	for (int y = 0; y < ui->stage_hgt; ++y) {
		for (int x = 0; x < ui->stage_wid; ++x) {
			switch (ui->stage[x][y]) {
			case WALL_H: printf("="); break;
			case WALL_V: printf("|"); break;
			case FIELD: printf(" "); break;
			case SNAKE:printf("\e[32mo\e[0m"); break;
			case FOOD: printf("\e[31m@\e[0m");
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
	system("clear");

	ui_display(ui);

	if (kbhit()) 
		game_set_key(ui->g, getch());

}


int ui_init(ui_t **ui, game_t *g)
{

	*ui = malloc(sizeof(ui_t));

	if (!(*ui)) {
		perror("malloc"); 
		return 1;
	}

	ui_set_game((*ui), g);
	char **stage = game_get_stage(g);

	ui_set_stage((*ui), stage);

	(*ui)->stage_wid = game_get_stage_wid(g);
	(*ui)->stage_hgt = game_get_stage_hgt(g);
	game_set_ui(g, *ui);

	open_termios();

	return 0;
}


void ui_free(ui_t *ui) 
{
	close_termios();
	free(ui);
}


