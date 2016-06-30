#include <stdio.h>
#include <stdlib.h>

#include "possmoves.h"
#include "move.h"

void
PossibleMoves_init(possmoves_t *pm)
{
  pm->wall_size = 0;
  pm->walk_size = 0;
}


possmoves_t *
PossibleMoves_clone(const possmoves_t *pm, possmoves_t *new)
{
  if (new == NULL)
  {
    new = malloc(sizeof(possmoves_t));
  }
  new->wall_size = pm->wall_size;
  new->walk_size = pm->walk_size;

  for (int i = 5; i < 5 + new->wall_size; i++)
  {
    GameMove_clone(&(pm->moves[i]), &(new->moves[i]));
  }

  for (int i = 0; i < new->walk_size; i++)
  {
    GameMove_clone(&(pm->moves[i]), &(new->moves[i]));
  }

  return new;
}

void
PossibleMoves_print(possmoves_t *pm)
{
  printf("Walk Moves: %i; Wall Moves: %i\n", pm->walk_size, pm->wall_size);
  for (int i = 0; i < 5 + pm->wall_size; i++)
  {
    if (i < 5 && i >= pm->walk_size) continue;
    printf("%i - ", i);
    GameMove_print(&(pm->moves[i]));
    printf("\n");
  }
}
