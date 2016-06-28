#include <stdio.h>

#include "strdup/strdup.h"

#include "move.h"

void
GameMove_init(gamemove_t *move, player_t player, int row, char col, walldir_t wall, char *srep)
{
  move->player = player;
  move->row = row;
  move->col = col;
  move->wall = wall;
  move->srep = strdup(srep);
}

gamemove_t *
GameMove_clone(const gamemove_t *move, gamemove_t *new)
{
  if (new == NULL)
    new = malloc(sizeof(gamemove_t));

  GameMove_init(new, move->player, move->row, move->col, move->wall, move->srep);
  return new;
}

void
GameMove_print(gamemove_t *move)
{
  printf("%s", move->srep);
}
