
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "maze_gen.h"

char* cross_list[] = {"  ", "╙ ", "═ ", "╝ ",
                      "╓ ", "║ ", "╗ ", "╣ ",
                      " ═", "╚═", "══", "╩═",
                      "╔═", "╠═", "╦═", "╬═"};
char* maze_string (Table* table) {
  Coordinate coordinate = {0,0};
  char* str = malloc(sizeof (char) * (table->h_maze * ( 1 + table->w_maze * 2 * 4) + 1));
  strcpy (str, "");
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
    strcat (str, "\n");
  }
  return str;
}

int main () {
  srandom(time(NULL));
  Table* table = maze_table_gen (9, 9);
  Coordinate kernel = {0,1};
  maze_realize (table, &kernel);

  char* str = maze_string (table);
  printf ("%s", str);
  maze_destroy(table);
  free(str);
}

/*
int main () {
  srandom(time(NULL));
  // Cell *cell = calloc (1, sizeof (cell));
  Table* table = maze_table_gen (99, 99);
  Coordinate kernel = {0,1};
  maze_realize (table, &kernel);

  printf("cell[2][1]的下n个: %d, %d, %d, %d; 往下%d个点. \n", table->rows[2][1]->next[0],
      table->rows[2][1]->next[1],table->rows[2][1]->next[2],table->rows[2][1]->next[3], 
      table->rows[2][1]->next_num);
  maze_destroy(table);
  return 0;
}
*/
