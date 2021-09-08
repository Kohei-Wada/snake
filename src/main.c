#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "game.h"


void usage(void)
{
	printf("[Usage] snake [-h]\n");
	printf("[Flags]\n");
	printf("-h    show this help\n");
	printf("\n");

	printf("[Operations]\n");
	printf("a : left\n"); 	
	printf("f : right\n");
	printf("e : up\n");	
	printf("d : down\n");
	printf("q : quit\n");
	printf("p : pause\n");

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

