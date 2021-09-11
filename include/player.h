#ifndef PLAYER_H
#define PLAYER_H

typedef struct player player_t;
typedef struct game game_t;
typedef struct snake snake_t;


char player_get_key(player_t *p);
void player_set_key(player_t *p, char key);

snake_t *player_get_snake(player_t *p);
void player_set_snake(player_t *p, snake_t *s);

int player_init(player_t **p, game_t *g);
void player_free(player_t *p);
int player_update(player_t *p);
void player_result(player_t *p);

#endif
