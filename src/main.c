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


int main(void)
{

	struct winsize size;
	if (ioctl(1, TIOCGWINSZ, &size) == -1) 
		return 1;

	game_t *g;
	snake_t *s;
	ui_t *ui;

	if (game_init(&g, size.ws_col, size.ws_row - 2))
		goto fail;
	if (ui_init(&ui, g)) 
		goto fail;

	game_loop(g);
	game_result(g);


  fail:
	ui_free(ui);
	game_free(g);

	return 0;
}

