#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "game.h"


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
	if (snake_init(&s, g, size.ws_col/2, size.ws_row /2)) 
		goto fail;

	game_loop(g);
	game_result(g);


  fail:
	snake_free(s);
	ui_free(ui);
	game_free(g);

	return 0;
}

