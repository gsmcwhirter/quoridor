#include <stdbool.h>
#include <stdlib.h>

#include "adjlist.h"

// adjlist_t *
// AdjList_create(const unsigned char total_nodes) //, const int max_degree)
// {
//   adjlist_t *al = malloc(sizeof(adjlist_t));
//   AdjList_init(al, total_nodes);
//
//   return al;
// }

void
AdjList_init(adjlist_t *al, const unsigned char total_nodes)
{
  al->total_nodes = total_nodes;
  al->max_degree = 4;
  al->degree = 0;
  al->neighbors[0] = -1;
  al->neighbors[1] = -1;
  al->neighbors[2] = -1;
  al->neighbors[3] = -1;
}


adjlist_t *
AdjList_clone(const adjlist_t *al, adjlist_t *al2)
{
  if (al2 == NULL)
  {
    al2 = malloc(sizeof(adjlist_t));
    AdjList_init(al2, al->total_nodes);
  }
  // adjlist_t *al2 = malloc(sizeof(adjlist_t));
  // al2->total_nodes = al->total_nodes;
  // al2->max_degree = al->max_degree;
  al2->degree = al->degree;
  // al2->neighbors = malloc(sizeof(int) * al2->max_degree);
  for (int i = 0; i < al2->degree; i++)
  {
    al2->neighbors[i] = al->neighbors[i];
  }

  return al2;
}


void
AdjList_addNeighbor(adjlist_t *al, int n)
{
  if (!AdjList_hasNeighbor(al, n))
  {
    // if (al->max_degree - al->degree <= 0)
    // {
    //   // Error!
    // }
    // else
    // {
      *(al->neighbors + al->degree) = n;
      al->degree++;
      AdjList_sortNeighbors(al);
    // }
  }
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
AdjList_hasNeighbor(adjlist_t *al, int n)
{
  if (AdjList_findNeighbor(al, n) < 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}


char
AdjList_findNeighbor(adjlist_t *al, unsigned char n)
{
  for (int i = 0; i < al->degree; i++)
  {
    if (al->neighbors[i] == n)
    {
      return i;
    }
  }

  return -1;
}


void
AdjList_removeNeighbor(adjlist_t *al, unsigned char n)
{
  char loc = AdjList_findNeighbor(al, n);
  if (loc >= 0)
  {
    al->neighbors[(unsigned char)loc] = -1;
    AdjList_sortNeighbors(al);
    al->degree--;
  }
}


void
AdjList_destroy(adjlist_t *al)
{
  // if (al != NULL)
  // {
  //   free(al);
  // }
}


// void
// AdjList_relabel(adjlist_t *al, int num_nodes, int *nodes)
// {
//   for (int i = 0; i < al->degree; i++)
//   {
//     bool found = false;
//     for (int j = 0; j < num_nodes; j++)
//     {
//       if (*(al->neighbors + i) == *(nodes + j))
//       {
//         *(al->neighbors + i) = j;
//         found = true;
//       }
//     }
//
//     if (!found)
//     {
//       int temp = *(al->neighbors + (al->degree - 1));
//       *(al->neighbors + (al->degree - 1)) = *(al->neighbors + i);
//       *(al->neighbors + i) = temp;
//       al->degree--;
//       i--;
//     }
//   }
// }
