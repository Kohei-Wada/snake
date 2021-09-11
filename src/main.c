#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"


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


void setup_options(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "hf:c:")) != -1) {
		switch (opt) {
		case 'h': usage(); break;
		default : usage(); break;
		}
	}
}


int main(int argc, char **argv)
{
	game_t *g;
	player_t *p;

	setup_options(argc, argv);

	if (game_init(&g))
		return 1;

	player_init(&p, g);

	game_attach_player(g, p);

	game_loop(g);
	player_result(p);

	game_detach_player(g, p);
	player_free(p);
	game_free(g);

	return 0;
}

