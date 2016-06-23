#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

#define SQUARES_SIZE 9
#define SQUARES_SIZE_SQ 81
#define WALLS_SIZE 8
#define WALLS_SIZE_SQ 64

#define PLAYER_1_COLOR "red"
#define PLAYER_2_COLOR "yellow"
#define LABEL_COLOR "cyan"

typedef enum player {
  PLAYER1 = 1,
  PLAYER2 = 2
} player_t;

typedef enum walldir {
  HORIZONTAL,
  VERTICAL,
  NONE
} walldir_t;

typedef struct Wall {
  int loc;
  walldir_t dir;
} wall_t;

typedef struct Board {
  graph_t *squares;
  unsigned long long wall_slots; // at least 64 bits each, need 64
  int player1; //node number
  int player2; // node number
} board_t;

int locToInt(int r, char c, int row_size);
int intToRow(int loc, int row_size);
int intToCol(int loc, int row_size);

board_t * Board_create();
board_t * Board_clone(const board_t *board);
void Board_destroy(board_t *board);
bool Board_isWallAtCorner(board_t *board, int r, char c);
void Board_setWallAtCorner(board_t *board, int r, char c);
void Board_print(board_t *board, player_t as_player);
bool Board_wallBetween(board_t *board, int r1, char c1, int r2, char c2);
bool Board_addWall(board_t *board, walldir_t walldir, int r, char c);
bool Board_movePlayer(board_t *board, player_t player, int r, char c);
bool Board_validWall(board_t *board, walldir_t walldir, int r, char c);


#endif
