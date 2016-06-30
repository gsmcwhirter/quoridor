#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "move.h"

#define SQUARES_SIZE 9
#define SQUARES_SIZE_SQ 81
#define WALLS_SIZE 8
#define WALLS_SIZE_SQ 64

#define PLAYER_1_COLOR "red"
#define PLAYER_2_COLOR "yellow"
#define LABEL_COLOR "cyan"

typedef struct Board {
  graph_t squares;
  unsigned long long wall_slots; // at least 64 bits each, need 64
  unsigned char player1; //node number
  unsigned char player2; // node number
} board_t;

unsigned char rcToLoc(unsigned char r, char c);
unsigned char locToRow(unsigned char loc);
unsigned char locToCol(unsigned char loc);

void Board_init(board_t *board);
board_t * Board_clone(const board_t *board, board_t *new_board);
bool Board_isWallAtCorner(const board_t *board, unsigned char r, char c);
void Board_setWallAtCorner(board_t *board, unsigned char r, char c);
void Board_print(const board_t *board, player_t as_player, gamemove_t *lastmove);
bool Board_wallBetween(const board_t *board, unsigned char r1, char c1, unsigned char r2, char c2);
bool Board_addWall(board_t *board, walldir_t walldir, unsigned char r, char c);
bool Board_movePlayer(board_t *board, player_t player, unsigned char r, char c);
bool Board_validWall(const board_t *board, walldir_t walldir, unsigned char r, char c);


#endif
