#include <stdlib.h>
#include <stdio.h>
//#include <stdbool.h>
#include <time.h> // 随机数种子

enum DIRECTION {none_dir, w_dir, a_dir, s_dir, d_dir};
typedef struct {
  enum DIRECTION pre;
  enum DIRECTION next[4];
  int next_num;
} Cell;

typedef Cell** Row; // Row是一个数组，存放Cell*
typedef struct {
  Row* rows;
  int h_maze;
  int w_maze;
} Table; 

// 0┌─── j
//  │i     x->[i][j]
typedef struct {
  int i; 
  int j; 
} Coordinate;

int find_list (int* list, int len, int x) {
  // 从整形数组中寻找整数x, 返回下标
  for (int i=0; i<len; i++) {
    if (list[i] == x) {return i;}
  }
  return -1;
}

void random_list_gen (int* list,int len) {
  // 生成随机不重复序列
  int temp_len = 0;
  while (temp_len != len) {
    int temp_elem = (random() % len) + 1;
    if (find_list (list, temp_len, temp_elem) == -1) {
      list[temp_len] = temp_elem;
      temp_len ++;
    }
  }
}

Table* maze_table_gen (int h_maze, int w_maze) {
  // 给定行数h_maze，列数w_maze，生成迷宫表格
  // table->rows [i][j]: i行, j列
  Table* table = malloc (sizeof(Table)); 
  table->rows = malloc (h_maze * sizeof(Row)); // 生成一个存放Row的数组
  table->h_maze = h_maze;
  table->w_maze = w_maze;
  for (int i = 0; i < h_maze; i++) {
    table->rows[i] = malloc (w_maze * sizeof(Cell*)); // 每个元素都是一个存放Cell*的数组
    for (int j = 0; j < w_maze; j++) {
      Cell* temp_cell = calloc (1, sizeof(Cell)); // 每个元素都是一个Cell*
      random_list_gen((int*)temp_cell->next, 4);
      table->rows[i][j] = temp_cell;
    }
  }
  return table;
}

void maze_destroy (Table* table) {
  // 用于释放Table类型空间
  for (int i = 0; i < table->h_maze; i++) {
    for (int j = 0; j < table->w_maze; j++) {
      free(table->rows[i][j]);
    }
    free(table->rows[i]);
  }
  free(table->rows);
  free(table);
}

Coordinate* coordinate_move (Table* table, Coordinate* coordinate, enum DIRECTION dir) {
  // 给定坐标和方向，可以返回下一坐标
  // 内置合法性判断，只返回合法坐标或者NULL指针
  int i = coordinate->i;
  int j = coordinate->j;
  switch (dir) {
    case w_dir: 
      i --;
      break;
    case a_dir:
      j --;
      break;
    case s_dir:
      i ++;
      break;
    case d_dir:
      j ++;
      break;
  }
  if ((i < 0 || i > (table->h_maze-1)) || 
      (j < 0 || j > (table->w_maze-1))) {
    // 越界检查
    return NULL;
  }
  Coordinate* next_coordinate = malloc(sizeof(Coordinate));
  next_coordinate->i = i;
  next_coordinate->j = j;
  return next_coordinate;
}

enum DIRECTION dir_opposite(enum DIRECTION dir) {
  // 方向取反
  return ((dir + 2 - 1) % 4) + 1;
}

void Random_DFS_explore (Table* table, Coordinate* coordinate, enum DIRECTION pre) {
  // sleep(1);
  Cell* cell = table->rows [coordinate->i][coordinate->j];
  if ((cell->pre != none_dir) || (cell->next_num > 0)) {
    // 此坐标必须未被打开过
    // printf("被打开过. @@@@@\n");
    return; 
  }
  cell->pre = pre;

  enum DIRECTION dir;
  Coordinate* next_coordinate = NULL;
  while (cell->next_num < 4) {
    dir = cell->next [cell->next_num];
    next_coordinate = coordinate_move (table, coordinate, dir);
    cell->next_num ++;

    if (next_coordinate == NULL) {
      // 判断合法性，清空检测到的无效方向
      cell->next[cell->next_num - 1] = 0;
      continue;
    }
    // printf("坐标: [%d, %d], 方向: %d, 已有尝试次数: %d; \n",coordinate->i, coordinate->j, dir, cell->next_num);
    // printf("下一个点: [%d , %d] ----- \n", next_coordinate->i, next_coordinate->j);
    Random_DFS_explore (table, next_coordinate, dir_opposite(dir));

    free(next_coordinate); // 善后处理
    next_coordinate = NULL;
  }
}

void maze_realize (Table* table, Coordinate* kernel) {
  // 从kernel这个坐标开始生长迷宫
  Random_DFS_explore (table, kernel, none_dir);
}

int main () {
  srandom(time(NULL));
  // Cell *cell = calloc (1, sizeof (cell));
  Table* table = maze_table_gen (3, 2);
  Coordinate kernel = {0,1};
  maze_realize (table, &kernel);

  printf("cell[2][1]的下n个: %d, %d, %d, %d; 往下%d个点. \n", table->rows[2][1]->next[0],
      table->rows[2][1]->next[1],table->rows[2][1]->next[2],table->rows[2][1]->next[3], 
      table->rows[2][1]->next_num);
  maze_destroy(table);
  return 0;
}

