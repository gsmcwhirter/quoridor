#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "board.h"

#define DEFAULT_WALLS 10
#define PLAYER1_TARGET 72
#define PLAYER2_TARGET 8

typedef struct GameState {
  board_t *board;
  player_t player;
  int player1_walls;
  int player2_walls;
} gamestate_t;

gamestate_t * GameState_create(board_t *board, player_t player);
gamestate_t * GameState_clone(const gamestate_t *state);
void GameState_destroy(gamestate_t *state);
void GameState_print(gamestate_t *state, player_t as_player);
bool GameState_moveCurrentPlayer(gamestate_t *state, int r, char c);
bool GameState_legalMove(gamestate_t *state, player_t player, int r, char c);
bool GameState_addWallCurrentPlayer(gamestate_t *state, walldir_t walldir, int r, char c);
bool GameState_legalWall(gamestate_t *state, player_t player, walldir_t walldir, int r, char c);
bool GameState_isGameOver(gamestate_t *state);
void GameState_togglePlayer(gamestate_t *state);

#endif
