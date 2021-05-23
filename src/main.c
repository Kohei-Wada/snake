#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "game.h"
#include "ui.h"
#include "snake.h"


void usage() 
{
	printf("[Usage] snake game\n");
	printf("[Operations]\n");
  	printf("   f : right\n");
   	printf("   e : up\n");
	printf("   d : down\n");
	printf("   a : left\n");
	printf("   q : quit\n");
	exit(0);
}


int main(int argc, char **argv)
{
	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) 
			usage();
		else usage();
	}
	
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

	ui_start(ui);
	game_loop(g);
	game_result(g);


  fail:
	snake_free(s);
	ui_free(ui);
	game_free(g);

	return 0;
}

