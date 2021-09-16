#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "observer.h"
#include "enemy.h"



void usage(void)
{
	fprintf(stderr, "[Usage] snake [-h]\n");
	fprintf(stderr, "[Flags]\n");
	fprintf(stderr, "-h    Show this help\n");
	fprintf(stderr, "-n N  If N is set to a value greater than 0,\n");
	fprintf(stderr, "      that may enemy snakes will be placed on the stage.\n");

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


void multi_play_mode(int n)
{
	game_t *g;
	player_t *p;

	enemy_t **enemys = malloc(sizeof(enemy_t*) * n);

	if (game_init(&g))
		return;

	player_init(&p, g, "player1");

	for (int i = 0; i < n; ++i) {
		enemy_init(&enemys[i], g, NULL);
		game_attach_observer(g, (observer_t *)enemys[i]);
	}

	game_attach_observer(g, (observer_t *)p);

	game_loop(g);
	player_result(p);

	for (int i = 0; i < n; ++i) 
		enemy_free(enemys[i]);

	player_free(p);
	game_free(g);

	free(enemys);
}


void single_play_mode()
{
	game_t *g;

	player_t *p;


	if (game_init(&g))
		return;

	player_init(&p, g, "player1");
	game_attach_observer(g, (observer_t *)p);
	game_loop(g);

	player_result(p);


	player_free(p);

	game_free(g);

}


int main(int argc, char **argv)
{
	int opt;
	int n_enemys;

	while ((opt = getopt(argc, argv, "hn:")) != -1) {
		switch (opt) {
		case 'h': 
			usage(); 
			break;

		case 'n': 
			n_enemys = atoi(optarg); 
			break;

		default : 
			usage(); 
			break;
		}
	}

	if (n_enemys > 1) 
		multi_play_mode(n_enemys);
	else
		single_play_mode();

	return 0;
}

