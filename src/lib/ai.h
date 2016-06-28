#ifndef AI_H
#define AI_H

#include "gamestate.h"
#include "history.h"

typedef struct MoveSequence {
  int score;
  int num_moves;
  int max_moves;
  gamemove_t moves[10];
} moveseq_t;

void MoveSequence_init(moveseq_t *ms);
bool MoveSequence_add(moveseq_t *ms, gamemove_t *move);
moveseq_t * MoveSequence_clone(const moveseq_t *ms, moveseq_t *new);
void MoveSequence_print(moveseq_t *ms);

typedef struct BestMoves {
  int size;
  int max_size;
  int score;
  moveseq_t **moves;
} bestmoves_t;

void BestMoves_init(bestmoves_t *bm);
void BestMoves_add(bestmoves_t *bm, moveseq_t *ms);
void BestMoves_expand(bestmoves_t *bm);
void BestMoves_destroy(bestmoves_t *bm);
void BestMoves_print(bestmoves_t *bm);

typedef struct PossibleMoves {
  int wall_size;
  int walk_size;
  gamemove_t moves[133];
} possmoves_t;

void PossibleMoves_init(possmoves_t *pm);
possmoves_t * PossibleMoves_clone(const possmoves_t *pm, possmoves_t *new);
void PossibleMoves_print(possmoves_t *pm);

typedef struct AIStage {
  player_t my_player;
  gamestate_t gamestate;
  gamehistory_t history;
  possmoves_t nextmoves;
  int stage_score;
  bool stage_score_defined;
  // struct AIStage *prev;
} aistage_t;

void AIStage_init(aistage_t *ais, gamestate_t *gamestate, gamehistory_t *history);
void AIStage_generatePossibleMoves(aistage_t *ais);

aistage_t * AIStage_advance(const aistage_t *ais, aistage_t *next, gamemove_t *move); //clone equiv
void AIStage_updatePossibleMoves(aistage_t *ais);

// void AIStage_destroy(aistage_t *ais);

int AIStage_evaluateGameState(aistage_t *ais);

bestmoves_t AIStage_bestMoves(aistage_t *ais, moveseq_t *ms, int lookahead);

#endif
