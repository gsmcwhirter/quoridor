#include <stdbool.h>
#include <stdlib.h>

#include "adjlist.h"
#include "graph.h"



void
Graph_init(graph_t *g, const unsigned char nodes, const unsigned char max_degree)
{
  g->nodes = nodes;
  g->max_degree = max_degree;

  for (int i = 0; i < nodes; i++)
  {
    g->node_names[i] = i;
  }

  for (int i = 0; i < nodes; i++)
  {
    AdjList_init(&(g->adj_lists[i]), nodes);
  }
}

graph_t *
Graph_clone(const graph_t *g, graph_t *g2)
{
  if (g2 == NULL)
  {
    g2 = malloc(sizeof(graph_t));
  }

  g2->nodes = g->nodes;
  g2->max_degree = g->max_degree;

  for (int i = 0; i < g2->nodes; i++)
  {
    g2->node_names[i] = g->node_names[i];
    AdjList_clone(&(g->adj_lists[i]), &(g2->adj_lists[i]));
  }

  return g2;
}


void
Graph_addEdge(graph_t *graph, unsigned char n1, unsigned char n2)
{
  AdjList_addNeighbor(&(graph->adj_lists[n1]), n2);
  AdjList_addNeighbor(&(graph->adj_lists[n2]), n1);
}


bool
Graph_hasEdge(const graph_t *graph, unsigned char n1, unsigned char n2)
{
  if (AdjList_hasNeighbor(&(graph->adj_lists[n1]), n2))
  {
    return true;
  }
  else
  {
    return false;
  }
}


void
Graph_removeEdge(graph_t *graph, unsigned char n1, unsigned char n2)
{
  AdjList_removeNeighbor(&(graph->adj_lists[n1]), n2);
  AdjList_removeNeighbor(&(graph->adj_lists[n2]), n1);
}


adjlist_t *
Graph_neighbors(graph_t *graph, unsigned char n)
{
  return &(graph->adj_lists[n]);
}
