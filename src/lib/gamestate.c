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
GameState_print(gamestate_t *state, player_t as_player)
{
  Board_print(state->board, as_player);
  printf("\n");
  // term_color("grey");
  printf("Turn of player: ");
  if (state->player == PLAYER1)
    term_color(PLAYER_1_COLOR);
  else
    term_color(PLAYER_2_COLOR);
  printf("%i", state->player);
  if (state->player == as_player)
    printf(" (YOUR TURN)");
  printf("\n");
  term_reset();
  printf("Player 1 Walls: ");
  term_color(PLAYER_1_COLOR);
  printf("%i\n", state->player1_walls);
  term_reset();
  printf("Player 2 Walls: ");
  term_color(PLAYER_2_COLOR);
  printf("%i\n", state->player2_walls);
  term_reset();
}


bool
GameState_moveCurrentPlayer(gamestate_t *state, int r, char c)
{
  if (GameState_legalMove(state, state->player, r, c))
  {
    Board_movePlayer(state->board, state->player, r, c);
    return true;
  }
  else
  {
    return false;
  }
}

bool
GameState_legalMove(gamestate_t *state, player_t player, int r, char c)
{
  int loc, loc_other, loc_inter;
  if (player == PLAYER1)
  {
    loc = state->board->player1;
    loc_other = state->board->player2;
  }
  else
  {
    loc = state->board->player2;
    loc_other = state->board->player1;
  }

  if (locToInt(r, c, SQUARES_SIZE) == loc_other) return false;

  int loc_r = intToRow(loc, SQUARES_SIZE);
  char loc_c = 'a' + intToCol(loc, SQUARES_SIZE);

  if(!Board_wallBetween(state->board, loc_r, loc_c, r, c))
    return true;
  // jump code
  else if (abs(loc_r - r) == 1 && abs((int)loc_c - (int)c) == 1)
  {
    int diff = (int)c - (int)loc_c;
    loc_inter = locToInt(loc_r, c, SQUARES_SIZE);
    if (loc_other == loc_inter &&
        Board_wallBetween(state->board, loc_r, c, loc_r, c+diff) &&
        !Board_wallBetween(state->board, loc_r, loc_c, loc_r, c) &&
        !Board_wallBetween(state->board, loc_r, c, r, c)) return true;

    diff = r - loc_r;
    loc_inter = locToInt(r, loc_c, SQUARES_SIZE);
    if (loc_other == loc_inter &&
        Board_wallBetween(state->board, r, loc_c, r+diff, loc_c) &&
        !Board_wallBetween(state->board, loc_r, loc_c, r, loc_c) &&
        !Board_wallBetween(state->board, r, loc_c, r, c)) return true;

    return false;
  }
  else if (loc_r - r == 0 && abs((int)loc_c - (int)c) == 2)
  {
    loc_inter = locToInt(loc_r, ((int)loc_c + (int)c)/2, SQUARES_SIZE);
    if (loc_other != loc_inter) return false;
    if (Board_wallBetween(state->board, loc_r, loc_c, r, (char)(((int)loc_c + (int)c)/2))) return false;
    if (Board_wallBetween(state->board, r, c, r, (char)(((int)loc_c + (int)c)/2))) return false;
    return true;
  }
  else if ((int)loc_c - (int)c == 0 && abs(loc_r - r) == 2)
  {
    loc_inter = locToInt((loc_r + r)/2, loc_c, SQUARES_SIZE);
    if (loc_other != loc_inter) return false;
    if (Board_wallBetween(state->board, loc_r, loc_c, (loc_r + r)/2, c)) return false;
    if (Board_wallBetween(state->board, r, c, (loc_r + r)/2, c)) return false;
    return true;
  }
  else
  {
    return false;
  }
}


bool
GameState_addWallCurrentPlayer(gamestate_t *state, walldir_t walldir, int r, char c)
{
  if (!GameState_legalWall(state, state->player, walldir, r, c))
  {
    // printf("fail1\n");
    return false;
  }

  if (!Board_addWall(state->board, walldir, r, c))
  {
    // printf("fail2\n");
    return false;
  }

  if (state->player == PLAYER1)
  {
    state->player1_walls--;
  }
  else
  {
    state->player2_walls--;
  }

  return true;
}


bool
GameState_legalWall(gamestate_t *state, player_t player, walldir_t walldir, int r, char c)
{
  if (player == PLAYER1 && state->player1_walls == 0) return false;
  if (player == PLAYER2 && state->player2_walls == 0) return false;
  // printf("fail3\n");
  if (!Board_validWall(state->board, walldir, r, c)) return false;
  // printf("fail4\n");
  return true;
  // TODO: pathing
}


bool
GameState_isGameOver(gamestate_t *state)
{
  int p1target = locToInt(9, 'a', SQUARES_SIZE);
  int p2target = locToInt(1, 'a' + SQUARES_SIZE - 1, SQUARES_SIZE);

  if (state->board->player1 >= p1target || state->board->player2 <= p2target)
    return true;
  else
    return false;

}


void
GameState_togglePlayer(gamestate_t *state)
{
  if (state->player == PLAYER1) state->player = PLAYER2;
  else state->player = PLAYER1;
}
