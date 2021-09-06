#ifndef UI_H
#define UI_H

#include "game.h"


typedef struct ui ui_t;


int ui_init(ui_t **ui, game_t *g);
void ui_free(ui_t *ui);

void ui_update(ui_t *ui);


#endif
