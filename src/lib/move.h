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

gamemove_t *GameMove_create(player_t player, int row, char col, walldir_t wall, char *srep);
gamemove_t *GameMove_clone(gamemove_t *move);
void GameMove_print(gamemove_t *move);
void GameMove_destroy(gamemove_t *move);

#endif
