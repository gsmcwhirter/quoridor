#ifndef ADJLIST_H
#define ADJLIST_H

#include <stdbool.h>

typedef struct AdjList {
  unsigned char total_nodes;
  unsigned char degree;
  unsigned char max_degree;
  char neighbors[4];
} adjlist_t;

void AdjList_init(adjlist_t *al, const unsigned char total_nodes);
adjlist_t * AdjList_clone(const adjlist_t *al, adjlist_t *al2);
void AdjList_addNeighbor(adjlist_t *al, unsigned char n);
void AdjList_sortNeighbors(adjlist_t *al);
bool AdjList_hasNeighbor(adjlist_t *al, unsigned char n);
char AdjList_findNeighbor(adjlist_t *al, unsigned char n);
void AdjList_removeNeighbor(adjlist_t *al, unsigned char n);

#endif
