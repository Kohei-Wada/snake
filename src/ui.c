#include <stdlib.h> 
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "getch.h"
#include "game.h"


typedef struct ui {
	game_t *g;
	char **stage;
	int stage_wid;
	int stage_hgt;
	int active;
} ui_t;


static void ui_display(ui_t *ui)
{

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
}


int ui_init(ui_t **ui, game_t *g)
{

	open_termios();
	*ui = malloc(sizeof(ui_t));

	if (!(*ui)) {
		perror("malloc"); 
		return 1;
	}

	(*ui)->g = g;
	(*ui)->stage = game_get_stage(g);

	game_stage_size(g, &(*ui)->stage_wid, &(*ui)->stage_hgt);

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
	ui->active = 1;

	while (ui->active) {
		system("clear");
		ui_display(ui);
	
		if (kbhit()) {
			char key = getch();
			game_key_add(ui->g, key);
			if (key == 'q') ui->active = 0;
		}
		usleep(70000);
	}

	return NULL;
}


void ui_start(ui_t *ui)
{
	pthread_t handle;
	pthread_create(&handle, NULL, ui_loop, ui);
	pthread_detach(handle);
}


