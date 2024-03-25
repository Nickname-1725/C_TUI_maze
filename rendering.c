
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>

#include "maze_gen.h"
#include "rendering.h"

enum COLOR_PAIR {playground_pair = 1, menu_light_pair, menu_dark_pair};
const char* cross_list[] = {"  ", "╙ ", "═ ", "╝ ",
                            "╓ ", "║ ", "╗ ", "╣ ",
                            " ═", "╚═", "══", "╩═",
                            "╔═", "╠═", "╦═", "╬═"};
char* maze_string (Table* table) {
  Coordinate coordinate = {0,0};
  // 行数 x (行首空格x1 + 列数 x 每格字符x2 x Unicode占字节x4) + 休止符x1
  char* str = malloc(sizeof (char) * (table->h_maze * ( 1 + table->w_maze * 2 * 4) + 1));
  strcpy (str, " "); // 行首空格
  for (coordinate.i = 0; coordinate.i < table->h_maze; coordinate.i++) {
    for (coordinate.j = 0; coordinate.j < table->w_maze; coordinate.j++) {
      int cross_index = 0;
      for (enum DIRECTION dir = w_dir; dir <= d_dir; dir++) {
        Coordinate* neighbour = coordinate_way(table, &coordinate, dir);
        if (neighbour == NULL) {
          continue; 
        }
        cross_index += 1<<(dir - 1);
        free (neighbour);
      }
      strcat (str, cross_list[cross_index]);
    }
    strcat (str, " "); // 换行 + 空格
  }
  return str;
}

void coordinate_screen_map (Coordinate* coordinate, int *y, int *x) {
  *y = coordinate->i;
  *x = coordinate->j*2 + 1;
}

void playground_size (Table* table, int *y, int *x) {
  *y = table->h_maze;
  *x = table->w_maze*2 + 1;
}

const int side_bar_width = 25;
void init_gameboard (Table* table, WINDOW** gmbrd_win, WINDOW** plygrnd_win, WINDOW** timrun_win, WINDOW** msg_win, WINDOW** tips_win) {
  int plygrnd_size_y;
  int plygrnd_size_x;
  playground_size(table, &plygrnd_size_y, &plygrnd_size_x);
  /* 新建居中窗口 */
  *gmbrd_win = newwin(
      plygrnd_size_y, plygrnd_size_x + side_bar_width,
      (LINES - plygrnd_size_y) / 2, (COLS - plygrnd_size_x - side_bar_width) / 2);

  *plygrnd_win = derwin (*gmbrd_win,
      plygrnd_size_y, plygrnd_size_x, 
      0, 0); // w_maze * 2 + 2 (行首空格, 换行符)
  *timrun_win = derwin (*gmbrd_win,
      1, side_bar_width, 
      0, plygrnd_size_x); // todo: 根据迷宫大小自动计算playground_win尺寸, 以及timerun_win的位置; 模块化窗口的初始及更新
  *msg_win = derwin (*gmbrd_win,
      2, side_bar_width,
      1, plygrnd_size_x);
  *tips_win = derwin (*gmbrd_win,
      plygrnd_size_y-1-2, side_bar_width, 
      3, plygrnd_size_x);
  wbkgd(*plygrnd_win, COLOR_PAIR(playground_pair));
  wbkgd(*timrun_win, COLOR_PAIR(menu_light_pair));
  wbkgd(*msg_win, COLOR_PAIR(menu_dark_pair));
  wbkgd(*tips_win, COLOR_PAIR(menu_light_pair));
}

/*
void coordinate_screen_move (WINDOW* win, Coordinate* obsolete, Coordinate* coordinate) {
  // todo: 解决从屏幕上获取宽字符问题，把玩家位置绘制成涂色标记(或许也可能针对节点重新打印字符串)
  int y_obsolete, x_obsolete;
  int y, x;
  char ch_obsolete, ch;
  coordinate_screen_map (obsolete, &y_obsolete, &x_obsolete);
  coordinate_screen_map (coordinate, &y, &x);
  
  ch_obsolete = mvwinch(win, y_obsolete, x_obsolete); //&& A_CHARTEXT;
  ch = mvwinch(win, y, x); //&& A_CHARTEXT;
  mvwaddch (win, y_obsolete, x_obsolete, ch_obsolete);
  attron(COLOR_PAIR(2));
  mvwaddch (win, y, x, ch);
  attroff(COLOR_PAIR(2));
  
  wmove(win, y, x);
  wrefresh(win);
}
*/

void coordinate_screen_move (WINDOW* win, Coordinate* coordinate) {
  int y, x;
  coordinate_screen_map (coordinate, &y, &x);
  wmove(win, y, x);
  wrefresh(win);
}

void timerun_print (WINDOW* win, WINDOW* tim_win, int time_ms) {
  int y, x;
  getyx(win, x, y);
  int cemi_sec = (time_ms % 1000) / 10 ;
  int sec = (time_ms / 1000) % 60;
  int min = (time_ms / 1000) / 60;
  mvwprintw(tim_win, 0, 0, "time: %02d:%02d.%02d", min, sec, cemi_sec);
  wmove(win, y, x);
  wrefresh(tim_win);
  wrefresh(win);
}

void init_TUI () {
  /* initialize curses */
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true); // 读取小键盘和功能键
  start_color();

  init_pair(playground_pair, COLOR_BLACK, COLOR_WHITE);
  init_pair(menu_light_pair, COLOR_BLACK, COLOR_CYAN);
  init_pair(menu_dark_pair, COLOR_WHITE, COLOR_BLUE);
  refresh(); // 必须refresh(), 否则无法显示非并标准窗口
}

