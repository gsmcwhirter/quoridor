#ifndef HISTORY_H
#define HISTORY_H

#include "board.h"
#include "gamestate.h"
#include "move.h"

typedef struct History {
  struct History *next;
  struct History *prev;
  gamemove_t move;
  bool move_set;
  gamestate_t state;
} gamehistory_t;

void GameHistory_init(gamehistory_t * h, const gamestate_t *state);
gamehistory_t * GameHistory_clone(const gamehistory_t *history, gamehistory_t *new);
void GameHistory_push(gamehistory_t *history, gamemove_t *move, gamestate_t *state);
gamehistory_t * GameHistory_get(gamehistory_t *history, int i);
gamehistory_t * GameHistory_last(gamehistory_t *history);
gamehistory_t * GameHistory_pop(gamehistory_t *history);
void GameHistory_print(gamehistory_t *history);
void GameHistory_destroy(gamehistory_t *history);


#endif
