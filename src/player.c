#include <stdlib.h>

#include "player.h"


typedef struct player {
	snake_t *s;
} player_t;




int player_init(player_t **p)
{
	*p = malloc(sizeof(player_t));
	return 0;
}


void player_free(player_t *p)
{
	free(p);
}


int player_update(player_t *p)
{
	return 0;
}
