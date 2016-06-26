#ifndef VISITED_H
#define VISITED_H

#include <stdbool.h>

#include "board.h"

typedef struct VisitedRecord {
  // bool *visited;
  // unsigned long long rec1;
  // unsigned long long rec2;
  char rec[SQUARES_SIZE_SQ];
} visitedrec_t;

// visitedrec_t * VisitedRecord_create();
void VisitedRecord_init(visitedrec_t *rec);
bool VisitedRecord_visited(visitedrec_t *rec, unsigned char square);
void VisitedRecord_setVisited(visitedrec_t *rec, unsigned char square, char dist);
char VisitedRecord_distance(visitedrec_t *rec, unsigned char square);

#endif
