#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

#define BLOCKED_SCORE -10000000
#define WIN_SCORE 1000000

typedef enum player {
  PLAYER1 = 1,
  PLAYER2 = 2
} player_t;

typedef enum walldir {
  HORIZONTAL,
  VERTICAL,
  NONE
} walldir_t;

typedef struct Wall {
  int loc;
  walldir_t dir;
} wall_t;

typedef struct GameMove {
  player_t player;
  int row;
  char col;
  walldir_t wall;
  // char *srep;
  int score;
} gamemove_t;

void GameMove_init(gamemove_t *move, player_t player, int row, char col, walldir_t wall);//, char *srep);
gamemove_t *GameMove_clone(const gamemove_t *move, gamemove_t *new);
void GameMove_print(const gamemove_t *move);
bool GameMove_same(const gamemove_t *m1, const gamemove_t *m2);


#endif
