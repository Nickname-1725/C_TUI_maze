#ifndef MAZE_GEN_H
#define MAZE_GEN_H

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

/* 函数声明 */
Table* maze_table_gen(int h_maze, int w_maze);
void maze_destroy (Table* table);
void maze_realize (Table* table, Coordinate* coordinate);
Coordinate* coordinate_way (Table* table, Coordinate* coordinate, enum DIRECTION dir);

#endif

