#include <stdlib.h>
#include <stdio.h>

enum DIRECTION {w_dir, a_dir, s_dir, d_dir};
typedef struct {
  int pre;
  int next[4];
  int next_num;
} Cell;

typedef Cell** Row; // Row是一个数组，存放Cell*
typedef Row* Table; // Table是一个树组，存放Row（数组的基地址）

Table maze_table_gen (int h_maze, int w_maze) {
  // 给定行数h_maze，列数w_maze，生成迷宫表格
  // table [i][j]: i行, j列
  Table table = malloc (h_maze * sizeof(Row)); // 生成一个存放Row的数组
  for (int i = 0; i < h_maze; i++) {
    table[i] = malloc (w_maze * sizeof(Cell*)); // 每个元素都是一个存放Cell*的数组
    for (int j = 0; j < w_maze; j++) {
      table[i][j] = malloc (sizeof(Cell)); // 每个元素都是一个Cell*
    }
  }
  return table;
}

int main () {
  // Cell *cell = calloc (1, sizeof (cell));
  Table table = maze_table_gen (3, 2);
  // printf("pre: %d; \nnext: %d, %d, %d, %d; \nnext_num: %d; \n", 
  //     cell->pre, cell->next[0],cell->next[1],cell->next[2],cell->next[3],
  //     cell->next_num);
  // free(cell);

  printf("cell[0][0]的下一个: %d; \n", table[0][0]->pre);
  free (table);
}

