#ifndef AI_H
#define AI_H

#include "gamestate.h"
#include "history.h"
#include "bestmoves.h"
#include "possmoves.h"

typedef struct AIStage {
  player_t my_player;
  gamestate_t gamestate;
  possmoves_t nextmoves;
  int stage_score;
  bool stage_score_defined;
} aistage_t;

typedef struct AIThreadArg {
  aistage_t *ais;
  gamehistory_t *history;
  gamemove_t *bestmove;
  int *last_lookahead;
} aithreadarg_t;

void AIStage_init(aistage_t *ais, gamestate_t *gamestate); //, gamehistory_t *history);
void AIStage_generatePossibleMoves(aistage_t *ais);
aistage_t * AIStage_advance(const aistage_t *ais, aistage_t *next, gamemove_t *move); //clone equiv
void AIStage_updatePossibleMoves(aistage_t *ais);
int AIStage_evaluateGameState(aistage_t *ais, gamehistory_t *history);
void AIStage_bestMove(aistage_t *ais, gamehistory_t *history, gamemove_t *result, int *lookahead);

#endif
