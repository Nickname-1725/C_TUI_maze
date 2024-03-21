/* 本代码用于创建冒险游戏 */

#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>

#define GRASS ' '
#define EMPTY '.'
#define WATER '~'
#define MOUNTAIN '^'
#define PLAYER '*'

#define GRASS_PAIR 1
#define EMPTY_PAIR 1
#define WATER_PAIR 2
#define MOUNTAIN_PAIR 3
#define PLAYER_PAIR 4

void draw_map(void) {
  int y, x;

  // 背景
  attron(COLOR_PAIR(GRASS_PAIR));
  for (y = 0; y < LINES; y++) {
    mvhline(y, 0, GRASS, COLS);
  }
  attroff(COLOR_PAIR(GRASS_PAIR));
  
  // 山和山道
  attron(COLOR_PAIR(MOUNTAIN_PAIR));
  for (x = COLS /2; x < COLS * 3 / 4; x++) {
    mvvline(0, x, MOUNTAIN, LINES);
  }
  attroff(COLOR_PAIR(MOUNTAIN_PAIR));

  attron(COLOR_PAIR(GRASS_PAIR));
  mvhline(LINES / 4, 0, GRASS, COLS);
  attroff(COLOR_PAIR(GRASS_PAIR));
  // 湖
  attron(COLOR_PAIR(WATER_PAIR));
  for (y = 1; y < LINES / 2; y++) {
    mvhline(y, 1, WATER, COLS / 3);
  }
  attroff(COLOR_PAIR(WATER_PAIR));
}

int is_move_okay(int y, int x) {
  int testch;
  testch = mvinch(y, x);
  return (((testch & A_CHARTEXT) == GRASS)
      || ((testch & A_CHARTEXT) == EMPTY));
}

int main() {
  int y, x;
  int ch;

  /* initialize curses */
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true); // 读取小键盘和功能键

  // 初始化颜色
  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
  init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
  init_pair(MOUNTAIN_PAIR, COLOR_BLACK, COLOR_WHITE);
  init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);

  clear();
  draw_map();

  /* 初始化玩家 */
  y = LINES - 1;
  x = 0;

  do {
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvaddch(y, x, PLAYER);
    attroff(COLOR_PAIR(PLAYER_PAIR));
    move(y, x);
    refresh();

    ch = getch();

    // 判断输入键
    switch (ch) {
      case KEY_UP: 
      case 'w' :
      case 'W' :
        if ((y>0) && is_move_okay(y-1, x)) {
          attron(COLOR_PAIR(EMPTY_PAIR));
          mvaddch(y, x, EMPTY);
          attroff(COLOR_PAIR(EMPTY_PAIR));
          y = y - 1;
        }
        break;
      case KEY_DOWN: 
      case 's' :
      case 'S' :
        if ((y<LINES - 1) && is_move_okay(y+1, x)) {
          attron(COLOR_PAIR(EMPTY_PAIR));
          mvaddch(y, x, EMPTY);
          attroff(COLOR_PAIR(EMPTY_PAIR));
          y = y + 1;
        }
        break;
      case KEY_LEFT: 
      case 'a' :
      case 'A' :
        if ((x>0) && is_move_okay(y, x - 1)) {
          attron(COLOR_PAIR(EMPTY_PAIR));
          mvaddch(y, x, EMPTY);
          attroff(COLOR_PAIR(EMPTY_PAIR));
          x = x - 1;
        }
        break;
      case KEY_RIGHT: 
      case 'd' :
      case 'D' :
        if ((x<COLS - 1) && is_move_okay(y, x+1)) {
          attron(COLOR_PAIR(EMPTY_PAIR));
          mvaddch(y, x, EMPTY);
          attroff(COLOR_PAIR(EMPTY_PAIR));
          x = x + 1;
        }
        break;
    }
  } while ((ch != 'q') && (ch != 'Q'));

  /* 绘制完成 */
  endwin();
  exit(0);
}
