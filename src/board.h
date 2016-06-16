#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdbool.h>

#define SQUARES_SIZE 9
#define WALLS_SIZE 19
#define WALLS_SIZE_SQ 361

typedef struct Board {
  bool *walls;
  int player1[2];
  int player2[2];
} board_t;

board_t * Board_create();
board_t * Board_clone(const board_t *board);
void Board_destroy(board_t *board);
void Board_print(board_t *board);


#endif
