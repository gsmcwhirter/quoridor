#ifndef AI_H
#define AI_H

#include "gamestate.h"
#include "history.h"

typedef struct PossibleMoves {
  int wall_size;
  gamemove_t **wall_moves;
  int walk_size;
  gamemove_t **walk_moves;
} possmoves_t;

possmoves_t * PossibleMoves_create();
possmoves_t * PossibleMoves_clone(possmoves_t *pm);
void PossibleMoves_destroy(possmoves_t *pm);

typedef struct AIStage {
  int my_player;
  gamestate_t *gamestate;
  gamehistory_t *history;
  possmoves_t *nextmoves;
  int stage_score;
  struct AIStage *prev;
} aistage_t;

aistage_t * AIStage_create(gamestate_t *gamestate, gamehistory_t *history);
void AIStage_generatePossibleMoves(aistage_t *ais);

aistage_t * AIStage_advance(aistage_t *ais, gamemove_t *move); //clone equiv
void AIStage_updatePossibleMoves(aistage_t *ais);

void AIStage_destroy(aistage_t *ais);

int AIStage_evaluateGameState(aistage_t *ais);

#endif
