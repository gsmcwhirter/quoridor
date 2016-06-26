#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "board.h"
#include "history.h"

#define DEFAULT_WALLS 10
#define PLAYER1_TARGET 72
#define PLAYER2_TARGET 8

typedef struct GameState {
  board_t board;
  player_t player;
  unsigned char player1_walls;
  unsigned char player2_walls;
} gamestate_t;

typedef enum MoveResult {
  OK,
  NOT_ENOUGH_WALLS,
  WALL_BLOCKED,
  PATH_BLOCKED,
  WALL_FAILED,
  MOVE_BLOCKED,
  ILLEGAL_MOVE,
  ILLEGAL_JUMP,
  MOVE_FAILED,
  NOT_YOUR_TURN
} moveresult_t;

char * moveDescription(moveresult_t res);

// gamestate_t * GameState_create(board_t *board, player_t player);
void GameState_init(gamestate_t *state, board_t *board, player_t player);
gamestate_t * GameState_clone(const gamestate_t *state, gamestate_t * newstate);
// void GameState_destroy(gamestate_t *state);
void GameState_print(gamestate_t *state, player_t as_player);
moveresult_t GameState_applyMove(gamestate_t *state, gamemove_t *move);
moveresult_t GameState_moveCurrentPlayer(gamestate_t *state, int r, char c);
moveresult_t GameState_legalMove(gamestate_t *state, player_t player, int r, char c);
moveresult_t GameState_addWallCurrentPlayer(gamestate_t *state, walldir_t walldir, int r, char c);
moveresult_t GameState_legalWall(gamestate_t *state, player_t player, walldir_t walldir, int r, char c);
bool GameState_isGameOver(gamestate_t *state);
void GameState_togglePlayer(gamestate_t *state);

#endif
