#include <stdbool.h>
#include <stdlib.h>

#include "visited.h"

// visitedrec_t *
// VisitedRecord_create()
// {
//   visitedrec_t *rec = malloc(sizeof(visitedrec_t));
//   //rec->visited = calloc(SQUARES_SIZE_SQ, sizeof(bool));
//   rec->rec1 = 0;
//   rec->rec2 = 0;
//
//   return rec;
// }

void
VisitedRecord_init(visitedrec_t *rec)
{
  for (int i = 0; i < SQUARES_SIZE_SQ; i++)
  {
    rec->rec[i] = -1;
  }
  // rec->rec1 = 0;
  // rec->rec2 = 0;
}


bool
VisitedRecord_visited(visitedrec_t *rec, unsigned char square)
{
  // unsigned long long r = rec->rec1;
  // if (square > 64)
  // {
  //   r = rec->rec2;
  //   square = square-64;
  // }
  //
  // //return *(rec->visited + square);
  // if ((r & (1ll<<square)) > 0) return true;
  // else return false;

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
