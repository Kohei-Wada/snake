#ifndef GAME_H
#define GAME_H

typedef struct player player_t;
typedef struct game game_t;
typedef struct board board_t;


board_t *game_get_board(game_t *g);

void game_attach_player(game_t *g, player_t *p);
void game_detach_player(game_t *g, player_t *p);

int game_init(game_t **g);
void game_free(game_t *g);
void game_loop(game_t *g);


void game_set_active(game_t *g, int a);
void game_set_pause(game_t *g, int p);
void game_result(game_t *g, player_t *p);
int game_get_pause(game_t *g);

#endif
