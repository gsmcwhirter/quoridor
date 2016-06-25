#ifndef HISTORY_H
#define HISTORY_H

#include "board.h"
#include "move.h"

typedef struct History {
  struct History *next;
  struct History *prev;
  gamemove_t *move;
} gamehistory_t;

gamehistory_t * GameHistory_create();
gamehistory_t * GameHistory_clone(gamehistory_t *history);
void GameHistory_push(gamehistory_t *history, gamemove_t *move);
gamehistory_t * GameHistory_get(gamehistory_t *history, int i);
gamehistory_t * GameHistory_pop(gamehistory_t *history);
void GameHistory_print(gamehistory_t *history);
void GameHistory_destroy(gamehistory_t *history);


#endif
