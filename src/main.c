#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "game.h"


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

	setup_options(argc, argv);

	if (game_init(&g))
		return 1;


	game_loop(g);
	game_result(g);
	game_free(g);

	return 0;
}

