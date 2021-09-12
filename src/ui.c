#include <stdlib.h> 
#include <stdio.h>

#include "ui.h"
#include "getch.h"
#include "snake.h"
#include "player.h"
#include "board.h"


typedef struct ui {
	player_t *player;
	board_t *board;
} ui_t;


player_t *ui_get_player(ui_t *ui)
{
	return  ui->player;
}


board_t *ui_get_board(ui_t *ui)
{
	return ui->board;
}


static void ui_display(ui_t *ui)
{
	board_t *b = ui_get_board(ui);
	for (int y = 0; y < board_get_hgt(b); ++y) {
		for (int x = 0; x < board_get_wid(b); ++x) 
			printf("%s", board_get_elem_shape(b, x, y));
		printf("\n");
	}
	player_print_status(ui_get_player(ui));
}


void ui_update(ui_t *ui)
{
	player_t *p = ui_get_player(ui);

	system("clear");
	ui_display(ui);

	if (kbhit()) 
		player_set_key(p, getch());
}


int ui_init(ui_t **ui, player_t *p, board_t *b)
{

	*ui = malloc(sizeof(ui_t));
	if (!(*ui)) 
		return 1;

	(*ui)->player = p;
	(*ui)->board  = b;

	open_termios();

	return 0;
}


void ui_free(ui_t *ui) 
{
	close_termios();
	free(ui);
}


