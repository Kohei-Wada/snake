#ifndef UI_H
#define UI_H

typedef struct player player_t;
typedef struct ui ui_t;
typedef struct board board_t;

int ui_init(ui_t **ui, player_t *p, board_t *b);
void ui_free(ui_t *ui);

void ui_update(ui_t *ui);

#endif
