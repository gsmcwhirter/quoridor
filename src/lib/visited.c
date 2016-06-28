#include <stdbool.h>
#include <stdlib.h>

#include "visited.h"

void
VisitedRecord_init(visitedrec_t *rec)
{
  for (int i = 0; i < SQUARES_SIZE_SQ; i++)
  {
    rec->rec[i] = -1;
  }
}


bool
VisitedRecord_visited(visitedrec_t *rec, unsigned char square)
{
  if (rec->rec[square] > -1) return true;
  else return false;
}


void
VisitedRecord_setVisited(visitedrec_t *rec, unsigned char square, char dist)
{
  // *(rec->visited + square) = true;
  // if (square < 64)
  // {
  //   rec->rec1 = (rec->rec1 | (1ll<<square));
  // }
  // else
  // {
  //   rec->rec2 = (rec->rec2 | (1ll<<(square-64)));
  // }
  rec->rec[square] = dist;
}

char
VisitedRecord_distance(visitedrec_t *rec, unsigned char square)
{
  return rec->rec[square];
}
