#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct GameMove {
  player_t player;
  int row;
  char col;
  walldir_t wall;
  char *srep;
} gamemove_t;

void GameMove_init(gamemove_t *move, player_t player, int row, char col, walldir_t wall, char *srep);
gamemove_t *GameMove_clone(const gamemove_t *move, gamemove_t *new);
void GameMove_print(gamemove_t *move);


#endif
