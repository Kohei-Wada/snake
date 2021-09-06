#include <stdlib.h> 
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "getch.h"
#include "game.h"
#include "ui.h"



static void ui_set_active(ui_t *ui, int active)
{
	ui->active = active;
}


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
			case SNAKE:printf("\e[32m+\e[0m"); break;
			case FOOD: printf("\e[31m@\e[0m");
			}
		}
		printf("\n");
	}

	if (g->pause)
		printf("pause : press 'p' to continue\n");
	else 
		printf("your length is %d\n", snake_len(g->snake));
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

	game_stage_size(g, &(*ui)->stage_wid, &(*ui)->stage_hgt);
	game_set_ui(g, *ui);

	open_termios();

	return 0;
}


void ui_free(ui_t *ui) 
{
	close_termios();
	free(ui);
}


void *ui_loop(void *v)
{
	ui_t *ui = v;
	ui_set_active(ui, 1);

	while (ui->active) {
		system("clear");
		ui_display(ui);
	
		if (kbhit()) {
			char key = getch();
			game_key_add(ui->g, key);
		}
		usleep(70000);
	}

	return NULL;
}


void ui_stop(ui_t *ui)
{
	ui_set_active(ui, 0);
}



void ui_start(ui_t *ui)
{
	pthread_create(&ui->handle, NULL, ui_loop, ui);
	pthread_detach(ui->handle);
}


