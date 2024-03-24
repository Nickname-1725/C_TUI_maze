#ifndef _RENDERING_H
#define _RENDERING_H

#include <ncurses.h>
#include "maze_gen.h"

void init_TUI();
void init_gameboard (Table* table, WINDOW** gmbrd_win, WINDOW** plygrnd_win, WINDOW** timrun_win, WINDOW** msg_win, WINDOW** tips_win);
char* maze_string (Table* table);
void coordinate_screen_move (WINDOW* win, Coordinate* coordinate);
void timerun_print(WINDOW* win, WINDOW* tim_win, int time_ms);

#endif

