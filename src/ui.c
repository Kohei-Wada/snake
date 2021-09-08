#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>

#include "getch.h"
#include "game.h"
#include "ui.h"
#include "snake.h"


typedef struct ui {
	game_t *g;
} ui_t;


static game_t* ui_get_game(ui_t *ui)
{
	return ui->g;
}


void ui_set_game(ui_t *ui, game_t *g)
{
	ui->g = g;
}


static void ui_display(ui_t *ui)
{
	game_t *g = ui_get_game(ui);
	char **stage = game_get_stage(g);

	for (int y = 0; y < game_get_stage_hgt(g); ++y) {

		for (int x = 0; x < game_get_stage_wid(g); ++x) {

			switch (stage[x][y]) {
			case FIELD   : printf(" ");            break;
			case WALL_H  : printf("=");            break;
			case WALL_V  : printf("|");            break;
			case FOOD    : printf("\e[31m@\e[0m"); break;
			case SNAKE   : printf("\e[32mo\e[0m"); break;

			case BLACKSNK     : printf("\e[30mo\e[0m"); break;
			case REDSNK       : printf("\e[31mo\e[0m"); break;
			case GREENSNK     : printf("\e[32mo\e[0m"); break;
			case YELLOWSNK    : printf("\e[33mo\e[0m"); break;
			case BLUESNK      : printf("\e[34mo\e[0m"); break;
			case MAGENTASNK   : printf("\e[35mo\e[0m"); break;
			case CYAANSNK     : printf("\e[36mo\e[0m"); break;
			case WHITESNK     : printf("\e[37mo\e[0m"); break;
						   
			}
		}
		printf("\n");
	}

	if (game_get_pause(g))
		printf("pause : press 'p' to continue\n");
	else 
		printf("your length is %d\n", snake_len(game_get_snake(g)));
}


void ui_update(ui_t *ui)
{
	game_t *g = ui_get_game(ui);

	system("clear");
	ui_display(ui);

	if (kbhit()) 
		game_set_key(g, getch());
}


int ui_init(ui_t **ui, game_t *g)
{

	*ui = malloc(sizeof(ui_t));
	if (!(*ui)) {
		perror("malloc"); 
		return 1;
	}

	ui_set_game((*ui), g);
	open_termios();
	return 0;
}


void ui_free(ui_t *ui) 
{
	close_termios();
	free(ui);
}


