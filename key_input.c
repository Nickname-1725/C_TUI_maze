
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "maze_gen.h"
#include "rendering.h"

enum GAME_STATE {exit_state, default_on_state, customed_on_state};

enum GAME_STATE key_input_loop (Table* table, Coordinate* start, Coordinate* end, WINDOW* win) {
  int ch;
  Coordinate* coordinate = malloc(sizeof(Coordinate));
  *coordinate = *start;
  Coordinate* coordinate_temp = NULL;
  do {
    coordinate_screen_move (win, coordinate);
    if ((coordinate->i == end->i) && (coordinate->j == end->j)) {
      return default_on_state;
    }

    ch = getch();
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
      default : 
        coordinate_temp = NULL;
        break;
    }
    if (coordinate_temp != NULL) {
      free(coordinate);
      coordinate = coordinate_temp;
    }
  } while ((ch != 'q') && (ch != 'Q'));
  free(coordinate_temp);
  return exit_state;
}

enum GAME_STATE ready_loop () {
  int ch;
  do {
    ch = getch();
    switch (ch) {
      case ' ' :
        return customed_on_state;
        break;
    }
  } while ((ch != 'q') && (ch != 'Q'));
  return exit_state;
}

void time_run (WINDOW* gameboard_win, WINDOW* timerun_win) {
  struct timespec start, end;
  long ms;
  //long sec;
  clock_gettime(CLOCK_REALTIME, &start);

  while(1) {
    usleep(20000);
    clock_gettime(CLOCK_REALTIME, &end);
    ms = (end.tv_nsec - start.tv_nsec) / 1000000;
    ms += (end.tv_sec - start.tv_sec) * 1000;
    timerun_print(gameboard_win, timerun_win, ms);
  }
}

int main () {
  srandom(time(NULL));

  init_TUI();

  /* 开始互动 */
  enum GAME_STATE state = customed_on_state;
  do {
    /* 迷宫的准备工作 */
    Table* table = maze_table_gen (20, 20);
    Coordinate kernel = {0,1};
    maze_realize (table, &kernel);
    
    Coordinate start_point = {0, 0};
    Coordinate end_point = {table->h_maze-1, table->w_maze-1};
    
    /* 游戏板的准备工作 */
    WINDOW* gameboard_win;
    WINDOW *playground_win, *timerun_win, *message_win, *tips_win;
    init_gameboard (table, &gameboard_win, &playground_win, &timerun_win, &message_win, &tips_win);

    maze_render(table, playground_win);
    if (state != default_on_state) {
      message_tips_print (message_win, "Press [space] key to start.");
    } else {
      message_tips_print (message_win, "You won, another game? ([space] key)");
    }
    message_tips_print (tips_win, "[q/Q] for quit.\n[p/P]: Comming soon.\n");
    timerun_print(gameboard_win, timerun_win, 0);

    cursor_reset();

    /* 开始互动 */
    state = ready_loop(); // 应该控制游戏退出
    if (state == exit_state) {break;}
    message_tips_print (message_win, "Dash to the right-bottom corner.");
    time_run(gameboard_win, timerun_win);

    //timerun_print(gameboard_win, timerun_win,123456);
    state = key_input_loop(table, &start_point, &end_point, gameboard_win);

    /* 结束 */
    maze_destroy(table);
  } while (state != exit_state);

  // 善后工作
  endwin();
  exit(0);
}

