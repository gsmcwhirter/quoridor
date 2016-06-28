#ifndef MOVE_H
#define MOVE_H

#define BLOCKED_SCORE -10000000

#include "board.h"

typedef struct GameMove {
  player_t player;
  int row;
  char col;
  walldir_t wall;
  // char *srep;
  int score;
} gamemove_t;

void GameMove_init(gamemove_t *move, player_t player, int row, char col, walldir_t wall);//, char *srep);
gamemove_t *GameMove_clone(const gamemove_t *move, gamemove_t *new);
void GameMove_print(const gamemove_t *move);
bool GameMove_same(const gamemove_t *m1, const gamemove_t *m2);


#endif
