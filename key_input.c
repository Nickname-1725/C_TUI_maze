
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "maze_gen.h"
#include "rendering.h"

void key_input_loop (Table* table, Coordinate* coordinate, WINDOW* win) {
  int ch;
  do {
    coordinate_screen_move (win, coordinate);

    ch = getch();
    Coordinate* coordinate_temp;
    // 判断输入键
    switch (ch) {
      case KEY_UP: 
      case 'w' :
      case 'W' :
        coordinate_temp = coordinate_way (table, coordinate, w_dir);
        break;
      case KEY_DOWN: 
      case 's' :
      case 'S' :
        coordinate_temp = coordinate_way (table, coordinate, s_dir);
        break;
      case KEY_LEFT: 
      case 'a' :
      case 'A' :
        coordinate_temp = coordinate_way (table, coordinate, a_dir);
        break;
      case KEY_RIGHT: 
      case 'd' :
      case 'D' :
        coordinate_temp = coordinate_way (table, coordinate, d_dir);
        break;
    }
    if (coordinate_temp != NULL) {
      free(coordinate);
      coordinate = coordinate_temp;
    }
  } while ((ch != 'q') && (ch != 'Q'));
}

int main () {
  srandom(time(NULL));

  init_TUI();
  /* 迷宫的准备工作 */
  Table* table = maze_table_gen (20, 20);
  Coordinate kernel = {0,1};
  maze_realize (table, &kernel);

  /* 游戏板的准备工作 */
  WINDOW* gameboard_win;
  WINDOW *playground_win, *timerun_win, *message_win, *tips_win;
  init_gameboard (table, &gameboard_win, &playground_win, &timerun_win, &message_win, &tips_win);

  char* str = maze_string (table);
  waddstr (playground_win, str); 

  waddstr (message_win, "Press [any] key to start.");
  waddstr (tips_win, "[q/Q] for quit.\n[p/P]: Comming soon.\n");

  wrefresh(gameboard_win);

  /* 开始互动 */
  Coordinate* start_point = malloc(sizeof(Coordinate));
  start_point->i = 0;
  start_point->j = 0;
  timerun_print(gameboard_win, timerun_win,123456);
  timerun_print(gameboard_win, timerun_win,123456);
  key_input_loop(table, start_point, gameboard_win);

  // 善后工作
  endwin();
  maze_destroy(table);
  free(str);
  exit(0);
}

