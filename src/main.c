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
	fprintf(stderr, "-h    show this help\n");
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


void multi_play_test()
{
	game_t *g;

	player_t *p;
	enemy_t *e1, *e2, *e3, *e4;

	if (game_init(&g))
		return;

	player_init(&p, g, "player1");
	enemy_init(&e1, g, "enemy1");
	enemy_init(&e2, g, "enemy2");
	enemy_init(&e3, g, "enemy3");
	enemy_init(&e4, g, "enemy4");

	game_attach_observer(g, (observer_t *)p);
	game_attach_observer(g, (observer_t *)e1);
	game_attach_observer(g, (observer_t *)e2);
	game_attach_observer(g, (observer_t *)e3);
	game_attach_observer(g, (observer_t *)e4);

	game_loop(g);

	player_result(p);


	player_free(p);
	enemy_free(e1);
	enemy_free(e2);
	enemy_free(e3);
	enemy_free(e4);

	game_free(g);
}


void single_play_test()
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


//global flag
int multi = 0;


void setup_options(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "hms")) != -1) {
		switch (opt) {
		case 'h': usage(); break;
		case 'm': multi = 1; break;
		case 's': multi = 0; break;
		default : usage(); break;
		}
	}
}


int main(int argc, char **argv)
{
	setup_options(argc, argv);

	if (multi)
		multi_play_test();
	else 
		single_play_test();

	return 0;
}

