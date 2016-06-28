#ifndef POSSMOVES_H
#define POSSMOVES_H

#include "move.h"

typedef struct PossibleMoves {
  int wall_size;
  int walk_size;
  gamemove_t moves[133];
} possmoves_t;

void PossibleMoves_init(possmoves_t *pm);
possmoves_t * PossibleMoves_clone(const possmoves_t *pm, possmoves_t *new);
void PossibleMoves_print(possmoves_t *pm);

#endif
