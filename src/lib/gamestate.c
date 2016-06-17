#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "gamestate.h"
#include "term/term.h"


gamestate_t *
GameState_create(board_t *board, player_t player)
{
  gamestate_t *gamestate = malloc(sizeof(gamestate_t));
  gamestate->board = board;
  gamestate->player = player;
  gamestate->player1_walls = DEFAULT_WALLS;
  gamestate->player2_walls = DEFAULT_WALLS;

  return gamestate;
}


gamestate_t *
GameState_clone(const gamestate_t *state)
{
  gamestate_t * newstate = malloc(sizeof(gamestate_t));
  newstate->board = Board_clone(state->board);
  newstate->player = state->player;
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


void
GameState_print(gamestate_t *state)
{
  Board_print(state->board);
  term_color("grey");
  printf("Turn of player: %i\n", state->player);
  printf("Player 1 Walls: %i\n", state->player1_walls);
  printf("Player 2 Walls: %i\n", state->player2_walls);
  term_reset();
}


bool
GameState_legalWall(gamestate_t *state, player_t player, walldir_t walldir, int r, int c)
{
  //TODO: pathing
  bool walls_left;
  if (player == PLAYER1)
  {
    walls_left = state->player1_walls > 0;
  }
  else
  {
    walls_left = state->player2_walls > 0;
  }
  return walls_left && Board_validWall(state->board, walldir, r, c);
}


void
GameState_addWall(gamestate_t *state, player_t player, walldir_t walldir, int r, int c)
{
  if (player == PLAYER1)
  {
    state->player1_walls--;
  }
  else
  {
    state->player2_walls--;
  }
  Board_addWall(state->board, walldir, r, c);
}


bool
GameState_legalMove(gamestate_t *state, player_t player, int r, int c)
{
  int pr, pc;
  if (player == PLAYER1)
  {
    pr = state->board->player1[0];
    pc = state->board->player1[1];
  }
  else
  {
    pr = state->board->player2[0];
    pc = state->board->player2[1];
  }
  return adjacentSquares(pr, pc, r, c) && !Board_wallBetween(state->board, pr, pc, r, c);
}


void
GameState_movePlayer(gamestate_t *state, player_t player, int r, int c)
{
  Board_movePlayer(state->board, player, r, c);
}
