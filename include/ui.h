#ifndef UI_H
#define UI_H

#include "game.h"



typedef struct ui {
	game_t *g;
	pthread_t handle;
	char **stage;
	int stage_wid;
	int stage_hgt;
	int active;
} ui_t;



int ui_init(ui_t **ui, game_t *g);
void ui_free(ui_t *ui);

void ui_update(ui_t *ui);
void ui_start(ui_t *ui);
void ui_stop(ui_t *ui);


#endif
