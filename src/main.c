#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "observer.h"
#include "enemy.h"



void usage(void)
{
	fprintf(stderr, "[Usage] snake [-h|-n N|-p X|-f N]\n");
	fprintf(stderr, "[Flags]\n");
	fprintf(stderr, "-h    Show this help\n");
	fprintf(stderr, "-n N  If N is set to a value greater than 0,\n");
	fprintf(stderr, "      that many enemy snakes will be placed on the stage.\n");
	fprintf(stderr, "      The enemys is too stupid to die soon, The enemys is set\n");
	fprintf(stderr, "      not to die.\n");
	fprintf(stderr, "-p X  set player name to X.\n");
	fprintf(stderr, "-f N  set number of foods to N.\n");

	fprintf(stderr, "\n");

	fprintf(stderr, "[Operations]\n");
	fprintf(stderr, "a : left\n"); 	
	fprintf(stderr, "f : right\n");
	fprintf(stderr, "e : up\n");	
	fprintf(stderr, "d : down\n");
	fprintf(stderr, "q : quit\n");
	fprintf(stderr, "p : pause\n");

	exit(0);
}


void multi_play_mode(int n, const char *pname, int n_foods)
{
	game_t *g;
	player_t *p;

	enemy_t **enemys = malloc(sizeof(enemy_t*) * n);

	if (game_init(&g))
		return;
	game_set_nfoods(g, n_foods);

	player_init(&p, g, pname);

	for (int i = 0; i < n; ++i) {
		enemy_init(&enemys[i], g, "enemy");
		game_attach_observer(g, (observer_t *)enemys[i]);
	}

	game_attach_observer(g, (observer_t *)p);

	game_loop(g);
	player_result(p);

	for (int i = 0; i < n; ++i) {
		enemy_free(enemys[i]);
	}

	player_free(p);
	game_free(g);

	free(enemys);
}


int main(int argc, char **argv)
{
	int opt, n_enemys = 0, n_foods = 10;

	const char *pname = "player1";

	while ((opt = getopt(argc, argv, "hn:p:f:")) != -1) {
		switch (opt) {
		case 'h': 
			usage(); 
			break;

		case 'n': 
			n_enemys = atoi(optarg); 
			break;

		case 'p':
			pname = (const char *)optarg;
			break;

		case 'f': 
			n_foods = atoi(optarg);
			break;

		default : 
			usage(); 
			break;
		}
	}

	multi_play_mode(n_enemys, pname, n_foods);

	return 0;
}

