#ifndef _RENDERING_H
#define _RENDERING_H

#include <ncurses.h>
#include "maze_gen.h"

void init_TUI();
void init_gameboard (Table* table, WINDOW** gmbrd_win, WINDOW** plygrnd_win, WINDOW** timrun_win, WINDOW** msg_win, WINDOW** tips_win);
void maze_render (Table* table, WINDOW* playground_win);
void coordinate_screen_move (WINDOW* win, Coordinate* coordinate);
void timerun_print (WINDOW* win, WINDOW* tim_win, long time_ms);
void message_tips_print (WINDOW* win, char* str);
void cursor_reset ();

#endif

