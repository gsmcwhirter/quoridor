#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"

adjlist_t *
AdjList_create(const int total_nodes) //, const int max_degree)
{
  adjlist_t *al = malloc(sizeof(adjlist_t));
  al->total_nodes = total_nodes;
  al->max_degree = 4;
  al->degree = 0;
  al->neighbors = malloc(sizeof(int) * 4); //max degree
  *(al->neighbors) = -1;
  *(al->neighbors + 1) = -1;
  *(al->neighbors + 2) = -1;
  *(al->neighbors + 3) = -1;

  return al;
}


adjlist_t *
AdjList_clone(adjlist_t *al)
{
  adjlist_t *al2 = AdjList_create(al->total_nodes);
  // adjlist_t *al2 = malloc(sizeof(adjlist_t));
  // al2->total_nodes = al->total_nodes;
  // al2->max_degree = al->max_degree;
  al2->degree = al->degree;
  // al2->neighbors = malloc(sizeof(int) * al2->max_degree);
  for (int i = 0; i < al2->degree; i++)
  {
    *(al2->neighbors + i) = *(al->neighbors + i);
  }

  return al2;
}


void
AdjList_addNeighbor(adjlist_t *al, int n)
{
  if (!AdjList_hasNeighbor(al, n))
  {
    if (al->max_degree - al->degree <= 0)
    {
      // Error!
    }
    else
    {
      *(al->neighbors + al->degree) = n;
      al->degree++;
      AdjList_sortNeighbors(al);
    }
  }
}

static __inline__ void sort4(int * d)
{
#define SWAP(x,y) if (d[y] > d[x]) { int tmp = d[x]; d[x] = d[y]; d[y] = tmp; }
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


int
AdjList_findNeighbor(adjlist_t *al, int n)
{
  for (int i = 0; i < al->degree; i++)
  {
    if (*(al->neighbors + i) == n)
    {
      return i;
    }
  }

  return -1;
}


void
AdjList_removeNeighbor(adjlist_t *al, int n)
{
  int loc = AdjList_findNeighbor(al, n);
  if (loc >= 0)
  {
    // int temp = *(al->neighbors + (al->degree - 1));
    // *(al->neighbors + (al->degree - 1)) = *(al->neighbors + loc);
    // *(al->neighbors + loc) = temp;
    *(al->neighbors + loc) = -1;
    AdjList_sortNeighbors(al);
    al->degree--;
  }
}


void
AdjList_destroy(adjlist_t *al)
{
  if (al != NULL)
  {
    free(al->neighbors);
    free(al);
  }
}


void
AdjList_relabel(adjlist_t *al, int num_nodes, int *nodes)
{
  for (int i = 0; i < al->degree; i++)
  {
    bool found = false;
    for (int j = 0; j < num_nodes; j++)
    {
      if (*(al->neighbors + i) == *(nodes + j))
      {
        *(al->neighbors + i) = j;
        found = true;
      }
    }

    if (!found)
    {
      int temp = *(al->neighbors + (al->degree - 1));
      *(al->neighbors + (al->degree - 1)) = *(al->neighbors + i);
      *(al->neighbors + i) = temp;
      al->degree--;
      i--;
    }
  }
}


graph_t *
Graph_create(const int nodes, const int max_degree)
{
  graph_t *g = malloc(sizeof(graph_t));
  g->nodes = nodes;
  g->max_degree = max_degree;
  g->node_names = malloc(sizeof(int) * nodes);
  for (int i = 0; i < nodes; i++)
  {
    *(g->node_names + i) = i;
  }
  g->adj_lists = malloc(sizeof(adjlist_t *) * nodes);
  for (int i = 0; i < nodes; i++)
  {
    *(g->adj_lists + i) = AdjList_create(nodes);//, max_degree);
  }

  return g;
}


graph_t *
Graph_clone(graph_t *g)
{
  graph_t *g2 = malloc(sizeof(graph_t));
  g2->nodes = g->nodes;
  g2->max_degree = g->max_degree;
  g2->node_names = malloc(sizeof(int) * g2->nodes);
  g2->adj_lists = malloc(sizeof(adjlist_t *) * g2->nodes);
  for (int i = 0; i < g2->nodes; i++)
  {
    *(g2->node_names + i) = *(g->node_names + i);
    *(g2->adj_lists + i) = AdjList_clone(*(g->adj_lists + i));
  }

  return g2;
}


void
Graph_addEdge(graph_t *graph, int n1, int n2)
{
  AdjList_addNeighbor(*(graph->adj_lists + n1), n2);
  AdjList_addNeighbor(*(graph->adj_lists + n2), n1);
}


bool
Graph_hasEdge(graph_t *graph, int n1, int n2)
{
  if (AdjList_hasNeighbor(*(graph->adj_lists + n1), n2))
  {
    return true;
  }
  else
  {
    return false;
  }
}


void
Graph_removeEdge(graph_t *graph, int n1, int n2)
{
  AdjList_removeNeighbor(*(graph->adj_lists + n1), n2);
  AdjList_removeNeighbor(*(graph->adj_lists + n2), n1);
}

graph_t *
Graph_subgraph(graph_t *g, int num_nodes, int* nodes)
{
  graph_t *ng = Graph_create(num_nodes, g->max_degree);
  for (int i = 0; i < num_nodes; i++)
  {
    *(ng->node_names + i) = *(nodes + i);
    *(ng->adj_lists + i) = AdjList_clone(*(g->adj_lists + *(nodes + i)));
    AdjList_relabel(*(ng->adj_lists + i), num_nodes, nodes);
  }
  return ng;
}


void
Graph_destroy(graph_t *g)
{
  if (g != NULL)
  {
    free(g->node_names);
    for (int i = 0; i < g->nodes; i++)
    {
      AdjList_destroy(*(g->adj_lists + i));
    }
    free(g->adj_lists);
    free(g);
  }
}

adjlist_t *
Graph_neighbors(graph_t *graph, int n)
{
  return *(graph->adj_lists + n);
}
