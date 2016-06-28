#ifndef BESTMOVES_H
#define BESTMOVES_H

// #include "moveseq.h"
#include "move.h"

typedef struct BestMoves {
  int size;
  // int max_size;
  int score;
  // moveseq_t *moves;
  gamemove_t moves[133];
} bestmoves_t;

void BestMoves_init(bestmoves_t *bm);
void BestMoves_reset(bestmoves_t *bm);
bestmoves_t * BestMoves_clone(const bestmoves_t *bm, bestmoves_t *new);
// void BestMoves_add(bestmoves_t *bm, moveseq_t *ms);
void BestMoves_add(bestmoves_t *bm, gamemove_t *ms, bool usemin);
// void BestMoves_expand(bestmoves_t *bm);
// void BestMoves_destroy(bestmoves_t *bm);
void BestMoves_print(const bestmoves_t *bm);

#endif
