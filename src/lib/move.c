#include <stdio.h>

// #include "strdup/strdup.h"

#include "move.h"

void
GameMove_init(gamemove_t *move, player_t player, int row, char col, walldir_t wall)//, char *srep)
{
  move->player = player;
  move->row = row;
  move->col = col;
  move->wall = wall;
  move->score = BLOCKED_SCORE;
  // move->srep = strdup(srep);
}

gamemove_t *
GameMove_clone(const gamemove_t *move, gamemove_t *new)
{
  if (new == NULL)
    new = malloc(sizeof(gamemove_t));

  GameMove_init(new, move->player, move->row, move->col, move->wall);//, move->srep);
  new->score = move->score;
  return new;
}

void
GameMove_print(const gamemove_t *move)
{
  if (move->wall == HORIZONTAL)
  {
    printf("%c%ih", move->col, move->row);
  }
  else if (move->wall == VERTICAL)
  {
    printf("%c%iv", move->col, move->row);
  }
  else
  {
    printf("%c%i", move->col, move->row);
  }
}

bool
GameMove_same(const gamemove_t *m1, const gamemove_t *m2)
{
  if (m1->player != m2->player) return false;
  if (m1->row != m2->row) return false;
  if (m1->col != m2->col) return false;
  if (m1->wall != m2->wall) return false;
  return true;
}
