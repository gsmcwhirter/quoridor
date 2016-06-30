#include <stdio.h>

#include "bestmoves.h"
#include "moveseq.h"

void
BestMoves_init(bestmoves_t *bm)
{
  bm->size = 0;
  bm->score = BLOCKED_SCORE;
}


void
BestMoves_reset(bestmoves_t *bm)
{
  bm->size = 0;
  bm->score = BLOCKED_SCORE;
}

bestmoves_t *
BestMoves_clone(const bestmoves_t *bm, bestmoves_t *new)
{
  if (new == NULL)
  {
    new = malloc(sizeof(bestmoves_t));
  }

  new->size = bm->size;
  new->score = bm->score;

  for (int i = 0; i < new->size; i++)
  {
    GameMove_clone(&(bm->moves[i]), &(new->moves[i]));
  }

  return new;
}

void
BestMoves_add(bestmoves_t *bm, gamemove_t *ms, bool maximize)
{
  // if (bm->size >= 133)
  //   printf("ARRAY INDEX OUT OF BOUNDS!\n");
  for (int i = 0; i < bm->size; i++)
  {
    if (GameMove_same(&(bm->moves[i]), ms))
    {
      if (maximize)
        bm->moves[i].score = (bm->moves[i].score > ms->score) ? bm->moves[i].score : ms->score;
      else
        bm->moves[i].score = (bm->moves[i].score < ms->score) ? bm->moves[i].score : ms->score;
      return;
    }
  }
  GameMove_clone(ms, &(bm->moves[bm->size]));
  bm->size++;
}


void
BestMoves_print(const bestmoves_t *bm)
{
  printf("Best score: %i\n", bm->score);
  for (int i = 0; i < bm->size; i++)
  {
    printf("\t");
    GameMove_print((bm->moves + i));
    printf( "(score=%i)", bm->moves[i].score);
    printf("\n");
  }
}
