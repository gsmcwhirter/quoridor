#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "board.h"

#define DEFAULT_WALLS 10

typedef struct GameState {
  board_t *board;
  bool player1_turn;
  int player1_walls;
  int player2_walls;
} gamestate_t;

gamestate_t * GameState_create(board_t *board, bool player1);
gamestate_t * GameState_clone(const gamestate_t *state);
void GameState_destroy(gamestate_t *state);

#endif
