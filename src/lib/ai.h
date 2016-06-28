#ifndef AI_H
#define AI_H

#include "gamestate.h"
#include "history.h"
#include "moveseq.h"
#include "bestmoves.h"
#include "possmoves.h"

typedef struct AIStage {
  player_t my_player;
  gamestate_t gamestate;
  // gamehistory_t history;
  possmoves_t nextmoves;
  int stage_score;
  bool stage_score_defined;
  // struct AIStage *prev;
} aistage_t;

void AIStage_init(aistage_t *ais, gamestate_t *gamestate); //, gamehistory_t *history);
void AIStage_generatePossibleMoves(aistage_t *ais);

aistage_t * AIStage_advance(const aistage_t *ais, aistage_t *next, gamemove_t *move); //clone equiv
void AIStage_updatePossibleMoves(aistage_t *ais);

// void AIStage_destroy(aistage_t *ais);

int AIStage_evaluateGameState(aistage_t *ais);

// void AIStage_bestMoves(aistage_t *ais, bestmoves_t *results, gamehistory_t *history, moveseq_t *ms, int lookahead);
void AIStage_bestMoves(aistage_t *ais, bestmoves_t *results, gamehistory_t *history, int lookahead);

#endif
