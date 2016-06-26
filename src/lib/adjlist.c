#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "adjlist.h"

void
AdjList_init(adjlist_t *al, const unsigned char total_nodes)
{
  #ifdef DEBUG
    printf("Initializing adjacency list...\n");
  #endif
  al->total_nodes = total_nodes;
  al->max_degree = 4;
  al->degree = 0;
  al->neighbors[0] = -1;
  al->neighbors[1] = -1;
  al->neighbors[2] = -1;
  al->neighbors[3] = -1;
  #ifdef DEBUG
    printf("done initializing.\n");
  #endif
}


adjlist_t *
AdjList_clone(const adjlist_t *al, adjlist_t *al2)
{
  #ifdef DEBUG
    printf("Cloning adjacency list...\n");
  #endif
  if (al2 == NULL)
  {
    #ifdef DEBUG
      printf("malloc for clone...\n");
    #endif
    al2 = malloc(sizeof(adjlist_t));
    AdjList_init(al2, al->total_nodes);
  }
  // adjlist_t *al2 = malloc(sizeof(adjlist_t));
  // al2->total_nodes = al->total_nodes;
  // al2->max_degree = al->max_degree;
  al2->degree = al->degree;
  // al2->neighbors = malloc(sizeof(int) * al2->max_degree);
  #ifdef DEBUG
    printf("copying neighbors...\n");
  #endif
  for (int i = 0; i < al2->degree; i++)
  {
    al2->neighbors[i] = al->neighbors[i];
  }
  #ifdef DEBUG
    printf("done cloning.\n");
  #endif

  return al2;
}


void
AdjList_addNeighbor(adjlist_t *al, unsigned char n)
{
  #ifdef DEBUG
    printf("Adding neighbor %i to adjacency list...\n", n);
  #endif
  if (!AdjList_hasNeighbor(al, n))
  {
    #ifdef DEBUG
      printf("recording...");
    #endif
    al->neighbors[al->degree] = n;
    al->degree++;
    #ifdef DEBUG
      printf("sorting adjacency list...\n");
    #endif
    AdjList_sortNeighbors(al);
    #ifdef DEBUG
      printf("done.\n");
    #endif
  }
  #ifdef DEBUG
  else
  {
    printf("already present.\n");
  }
  #endif
}

static __inline__ void sort4(char d[])
{
#define SWAP(x,y) if (d[y] > d[x]) { unsigned char tmp = d[x]; d[x] = d[y]; d[y] = tmp; }
SWAP(0, 1);
SWAP(2, 3);
SWAP(0, 2);
SWAP(1, 3);
SWAP(1, 2);
#undef SWAP
}

void
AdjList_sortNeighbors(adjlist_t *al)
{
  sort4(al->neighbors);
}

bool
AdjList_hasNeighbor(adjlist_t *al, unsigned char n)
{
  #ifdef DEBUG
    printf("Checking if %i is a neighbor...\n", n);
  #endif
  if (AdjList_findNeighbor(al, n) < 0)
  {
    #ifdef DEBUG
      printf("not a neighbor.\n");
    #endif
    return false;
  }
  else
  {
    #ifdef DEBUG
      printf("is a neighbor.\n");
    #endif
    return true;
  }
}


char
AdjList_findNeighbor(adjlist_t *al, unsigned char n)
{
  #ifdef DEBUG
    printf("Finding index of neighbor %i...\n", n);
  #endif
  for (int i = 0; i < al->degree; i++)
  {
    if (al->neighbors[i] == n)
    {
      #ifdef DEBUG
        printf("found at %i.\n", i);
      #endif
      return i;
    }
  }

  #ifdef DEBUG
    printf("failed.\n");
  #endif

  return -1;
}


void
AdjList_removeNeighbor(adjlist_t *al, unsigned char n)
{
  #ifdef DEBUG
    printf("Removing neighbor %i...\n", n);
  #endif
  char loc = AdjList_findNeighbor(al, n);
  if (loc >= 0)
  {
    #ifdef DEBUG
      printf("removing neighbor...\n");
    #endif
    al->neighbors[(unsigned char)loc] = -1;
    #ifdef DEBUG
      printf("sorting adjacency list...\n");
    #endif
    AdjList_sortNeighbors(al);
    al->degree--;
    #ifdef DEBUG
      printf("done.\n");
    #endif
  }
  #ifdef DEBUG
  else
    printf("not a neighbor.\n");
  #endif
}
