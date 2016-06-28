#include <stdio.h>

#include "bestmoves.h"
#include "moveseq.h"

void
BestMoves_init(bestmoves_t *bm)
{
  // bestmoves_t *bm = malloc(sizeof(bestmoves_t));
  bm->size = 0;
  // bm->max_size = 32;
  bm->score = BLOCKED_SCORE;
  // bm->moves = malloc(sizeof(moveseq_t) * (bm->max_size));
  // return bm;
}


void
BestMoves_reset(bestmoves_t *bm)
{
  bm->size = 0;
  // bm->max_size = 32;
  bm->score = BLOCKED_SCORE;
  // free(bm->moves);
  // bm->moves = malloc(sizeof(moveseq_t) * (bm->max_size));
}

bestmoves_t *
BestMoves_clone(const bestmoves_t *bm, bestmoves_t *new)
{
  if (new == NULL)
  {
    new = malloc(sizeof(bestmoves_t));
    // new->moves = NULL;
  }

  new->size = bm->size;
  // new->max_size = bm->max_size;
  new->score = bm->score;

  // if (new->moves != NULL)
  // {
  //   free(new->moves);
  // }

  // new->moves = malloc(sizeof(moveseq_t) * (new->max_size));
  for (int i = 0; i < new->size; i++)
  {
    // MoveSequence_clone((bm->moves + i), (new->moves + i));
    GameMove_clone(&(bm->moves[i]), &(new->moves[i]));
  }

  return new;
}

void
// BestMoves_add(bestmoves_t *bm, moveseq_t *ms)
BestMoves_add(bestmoves_t *bm, gamemove_t *ms, bool usemin)
{
  // if (bm->size >= bm->max_size)
  // {
  //   #ifdef DEBUGAI
  //     printf("Expanding BestMoves\n");
  //   #endif
  //   BestMoves_expand(bm);
  // }

  for (int i = 0; i < bm->size; i++)
  {
    if (GameMove_same(&(bm->moves[i]), ms))
    {
      if (usemin)
        bm->moves[i].score = ((bm->moves[i].score > ms->score) ? bm->moves[i].score : ms->score);
      else
          bm->moves[i].score = ((bm->moves[i].score < ms->score) ? bm->moves[i].score : ms->score);
      return;
    }
  }

  GameMove_clone(ms, &(bm->moves[bm->size]));
  // #ifdef DEBUGAI
  //   printf("(add) cloning ms\n");
  // #endif
  // MoveSequence_clone(ms, (bm->moves + bm->size));
  // // *(bm->moves + bm->size) = ms;
  bm->size++;
}

// void
// BestMoves_expand(bestmoves_t *bm)
// {
//   moveseq_t *old = bm->moves;
//   bm->moves = malloc(sizeof(moveseq_t) * (bm->max_size * 2));
//   bm->max_size = bm->max_size * 2;
//   for (int i = 0; i < bm->size; i++)
//   {
//     MoveSequence_clone((old + i), (bm->moves + i));
//     // *(bm->moves + i) = *(old + i);
//     // *(old + i) = NULL;
//   }
//   free(old);
// }


void
BestMoves_print(const bestmoves_t *bm)
{
  printf("Best score: %i\n", bm->score);
  for (int i = 0; i < bm->size; i++)
  {
    printf("\t");
    GameMove_print((bm->moves + i));
    printf("\n");
  }
}


// void
// BestMoves_destroy(bestmoves_t *bm)
// {
//   if (bm != NULL)
//   {
//     // for (int i = 0; i < bm->size; i++)
//     // {
//     //   // MoveSequence_destroy(*(bm->moves + i));
//     //   *(bm->moves + i) = NULL;
//     // }
//     free(bm->moves);
//     // free(bm);
//   }
// }
