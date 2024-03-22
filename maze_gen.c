#include <stdlib.h>
#include <stdio.h>
//#include <stdbool.h>
#include <time.h> // 随机数种子

enum DIRECTION {w_dir, a_dir, s_dir, d_dir};
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

int main () {
  srandom(time(NULL));
  // Cell *cell = calloc (1, sizeof (cell));
  Table* table = maze_table_gen (3, 2);

  printf("cell[0][0]的下n个: %d, %d, %d, %d; \n", table->rows[0][0]->next[0],
      table->rows[0][0]->next[1],table->rows[0][0]->next[2],table->rows[0][0]->next[3]);
  maze_destroy(table);
  return 0;
}

