#ifndef HISTORY_H
#define HISTORY_H

#include "board.h"

typedef struct GameMove {
  player_t player;
  int row;
  char col;
  walldir_t wall;
} gamemove_t;

gamemove_t *GameMove_create(player_t player, int row, char col, walldir_t wall);
void GameMove_print(gamemove_t *move);
void GameMove_destroy(gamemove_t *move);

typedef struct History {
  struct History *next;
  struct History *prev;
  gamemove_t *move;
} gamehistory_t;

gamehistory_t * GameHistory_create();
void GameHistory_push(gamehistory_t *history, gamemove_t *move);
gamehistory_t * GameHistory_get(gamehistory_t *history, int i);
gamehistory_t * GameHistory_pop(gamehistory_t *history);
void GameHistory_print(gamehistory_t *history);
void GameHistory_destroy(gamehistory_t *history);


#endif
