#include <stdio.h>

#include "move.h"
#include "strdup/strdup.h"

gamemove_t *
GameMove_create(player_t player, int row, char col, walldir_t wall, char *srep)
{
  gamemove_t *move = malloc(sizeof(gamemove_t));
  move->player = player;
  move->row = row;
  move->col = col;
  move->wall = wall;
  move->srep = strdup(srep);

  return move;
}

gamemove_t *
GameMove_clone(gamemove_t *move)
{
  return GameMove_create(move->player, move->row, move->col, move->wall, strdup(move->srep));
}

void
GameMove_print(gamemove_t *move)
{
  // printf("%c%i", move->col, move->row);
  // if (move->wall == HORIZONTAL)
  // {
  //   printf("h");
  // }
  // else if (move->wall == VERTICAL)
  // {
  //   printf("v");
  // }
  printf("%s", move->srep);
}

void
GameMove_destroy(gamemove_t *move)
{
  if (move != NULL)
  {
    free(move);
  }
}
