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
  bool stage_score_defined;
  struct AIStage *prev;
} aistage_t;

aistage_t * AIStage_create(gamestate_t *gamestate, gamehistory_t *history);
void AIStage_generatePossibleMoves(aistage_t *ais);

aistage_t * AIStage_advance(aistage_t *ais, gamemove_t *move); //clone equiv
void AIStage_updatePossibleMoves(aistage_t *ais);

void AIStage_destroy(aistage_t *ais);

int AIStage_evaluateGameState(aistage_t *ais);

typedef struct MoveSequence {
  int score;
  int num_moves;
  int max_moves;
  gamemove_t **moves;
} moveseq_t;

moveseq_t *MoveSequence_create();
void MoveSequence_add(moveseq_t *ms, gamemove_t *move);
void MoveSequence_expand(moveseq_t *ms);
void MoveSequence_destroy(moveseq_t *bm);

typedef struct BestMoves {
  int size;
  int max_size;
  int score;
  moveseq_t **moves;
} bestmoves_t;

bestmoves_t * BestMoves_create();
void BestMoves_add(bestmoves_t *bm, moveseq_t *ms);
void BestMoves_expand(bestmoves_t *bm);
void BestMoves_destroy(bestmoves_t *bm);

#endif
