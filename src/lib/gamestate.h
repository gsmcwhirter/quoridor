#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "board.h"

#define DEFAULT_WALLS 10

typedef struct GameState {
  board_t *board;
  player_t player;
  int player1_walls;
  int player2_walls;
} gamestate_t;

typedef struct GameMove {
  player_t player;
  int row;
  int col;
  walldir_t wall;
} gamemove_t;

gamestate_t * GameState_create(board_t *board, player_t player);
gamestate_t * GameState_clone(const gamestate_t *state);
void GameState_destroy(gamestate_t *state);
void GameState_print(gamestate_t *state);
bool GameState_legalWall(gamestate_t *state, player_t player, walldir_t walldir, int r, int c);
void GameState_addWall(gamestate_t *state, player_t player, walldir_t walldir, int r, int c);
bool GameState_legalMove(gamestate_t *state, player_t player, int r, int c);
void GameState_movePlayer(gamestate_t *state, player_t player, int r, int c);

#endif
