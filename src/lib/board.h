#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdbool.h>

#define SQUARES_SIZE 9
#define WALLS_SIZE 19
#define WALLS_SIZE_SQ 361

#define PLAYER_1_COLOR "red"
#define PLAYER_2_COLOR "yellow"
#define LABEL_COLOR "cyan"
#define WALL_CHARACTER_H 205
#define WALL_CHARACTER_V 186
#define WALL_CHARACTER_C 206

typedef enum player {
  PLAYER1 = 1,
  PLAYER2 = 2
} player_t;

typedef enum walldir {
  NONE = 0,
  HORIZONTAL = 1,
  VERTICAL = 2
} walldir_t;

typedef struct Board {
  bool *walls;
  int player1[2];
  int player2[2];
} board_t;

board_t * Board_create();
board_t * Board_clone(const board_t *board);
void Board_destroy(board_t *board);
void Board_print(board_t *board);
void Board_addWall(board_t *board, walldir_t walldir, int r, int c);
bool Board_validWall(board_t *board, walldir_t walldir, int r, int c);
void Board_movePlayer(board_t *board, player_t player, int r, int c);
bool Board_wallBetween(board_t *board, int r1, int c1, int r2, int c2);

bool adjacentSquares(int r1, int c1, int r2, int c2);

#endif
