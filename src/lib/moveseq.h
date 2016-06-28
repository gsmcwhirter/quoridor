#ifndef MOVESEQ_H
#define MOVESEQ_H

#define BLOCKED_SCORE_OLD -10000000

#include "move.h"

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

#endif
