#ifndef UI_H
#define UI_H

#include "game.h"
#include "player.h"

typedef struct ui ui_t;

int ui_init(ui_t **ui, game_t *g, player_t *p);
void ui_free(ui_t *ui);

void ui_update(ui_t *ui);


#endif
