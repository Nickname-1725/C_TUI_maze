
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include "maze_gen.h"

char* cross_list[] = {"  ", "╙ ", "═ ", "╝ ",
                      "╓ ", "║ ", "╗ ", "╣ ",
                      " ═", "╚═", "══", "╩═",
                      "╔═", "╠═", "╦═", "╬═"};
char* maze_string (Table* table) {
  Coordinate coordinate = {0,0};
  // 行数 x (行首空格x1 + 行末换行x1 + 列数 x 每格字符x2 x Unicode占字节x4) + 休止符x1
  char* str = malloc(sizeof (char) * (table->h_maze * ( 1 + 1 + table->w_maze * 2 * 4) + 1));
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
    strcat (str, "\n "); // 换行 + 空格
  }
  return str;
}

int main () {
  srandom(time(NULL));

  int y, x;
  int ch;

  /* initialize curses */
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true); // 读取小键盘和功能键

  /* 迷宫的准备工作 */
  Table* table = maze_table_gen (9, 9);
  Coordinate kernel = {0,1};
  maze_realize (table, &kernel);

  WINDOW* win = newwin(9, 20, 1, 1); // w_maze * 2 + 2 (行首空格, 换行符)
  refresh(); // 必须refresh(), 否则无法显示非并标准窗口
  box(win, 0, 0);

  char* str = maze_string (table);
  wprintw (win, "%s", str);
  //printw ("%s", str);
  wrefresh(win);

  getch();

  // 善后工作
  endwin();
  maze_destroy(table);
  free(str);
  exit(0);
}

