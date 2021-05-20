#include "game.h"
#include "ui.h"
#include "snake.h"


#include <stddef.h>
#include <sys/ioctl.h>

int main(void)
{

	struct winsize size;
	if (ioctl(1, TIOCGWINSZ, &size) == -1) 
		return 1;

	game_t *g;
	snake_t *s;
	ui_t *ui;

	game_init(&g, size.ws_col, size.ws_row - 2);
	ui_init(&ui, g);
	ui_start(ui);
	snake_init(&s, g);

	game_loop(g);

	snake_free(s);
	ui_free(ui);
	game_free(g);


	return 0;
}

