#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "gamestate.h"

gamestate_t *
GameState_create(board_t *board, bool player1)
{
  gamestate_t *gamestate = malloc(sizeof(gamestate_t));
  gamestate->board = board;
  gamestate->player1_turn = player1;
  gamestate->player1_walls = DEFAULT_WALLS;
  gamestate->player2_walls = DEFAULT_WALLS;

  return gamestate;
}


gamestate_t *
GameState_clone(const gamestate_t *state)
{
  gamestate_t * newstate = malloc(sizeof(gamestate_t));
  newstate->board = Board_clone(state->board);
  newstate->player1_turn = state->player1_turn;
  newstate->player1_walls = state->player1_walls;
  newstate->player2_walls = state->player2_walls;

  return newstate;
}


void
GameState_destroy(gamestate_t *state)
{
  if (state != NULL)
  {
    Board_destroy(state->board);
    free(state);
  }
}
