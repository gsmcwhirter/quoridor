#include <stdio.h>

#include "moveseq.h"
#include "move.h"

void
MoveSequence_init(moveseq_t *ms)
{
  // moveseq_t *bm = malloc(sizeof(moveseq_t));
  ms->num_moves = 0;
  ms->max_moves = 8;
  ms->score = BLOCKED_SCORE;
  // bm->moves = malloc(sizeof(gamemove_t *) * bm->max_moves);

  // return bm;
}

moveseq_t *
MoveSequence_clone(const moveseq_t *old, moveseq_t *ms)
{
  if (ms == NULL)
    ms = malloc(sizeof(moveseq_t));

  ms->num_moves = old->num_moves;
  ms->max_moves = old->max_moves;
  ms->score = old->score;
  // ms->moves = malloc(sizeof(gamemove_t *) * old->max_moves);

  for (int i = 0; i < old->num_moves; i++)
  {
    GameMove_clone(&(old->moves[i]), &(ms->moves[i]));
  }

  return ms;
}

bool
MoveSequence_add(moveseq_t *ms, gamemove_t *move)
{
  if (ms->num_moves == ms->max_moves)
  {
    return false;
  }

  GameMove_clone(move, &(ms->moves[ms->num_moves]));
  // *(ms->moves + ms->num_moves) = move;
  ms->num_moves++;
  return true;
}


void
MoveSequence_print(moveseq_t *ms)
{
  printf("(L=%i)", ms->num_moves);
  for (int i = 0; i < ms->num_moves; i++)
  {
    printf(",");
    GameMove_print(&(ms->moves[i]));
  }
  printf("\n");
}
