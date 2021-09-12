#ifndef ENEMY_H
#define ENEMY_H


typedef struct enemy enemy_t;
typedef struct game game_t;

int enemy_init(enemy_t **e, game_t *g, const char *name);
void enemy_free(enemy_t *e);

#endif
