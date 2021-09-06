#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "game.h"


void usage(void)
{

	printf("inside usage\n");
	exit(0);

}


int main(int argc, char **argv)
{

	game_t *g;


	int opt;
	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h': usage(); break;
		default : usage(); break;
		}
	}


	struct winsize size;
	if (ioctl(1, TIOCGWINSZ, &size) == -1) 
		return 1;


	if (game_init(&g, size.ws_col, size.ws_row - 2))
		goto fail;


	game_loop(g);
	game_result(g);


  fail:
	game_free(g);
	return 0;
}

