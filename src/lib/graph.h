#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#include "adjlist.h"

typedef struct Graph {
  unsigned char nodes;
  unsigned char max_degree;
  unsigned char node_names[81];
  adjlist_t adj_lists[81];
} graph_t;

void Graph_init(graph_t *g, const unsigned char nodes, const unsigned char max_degree);
graph_t * Graph_clone(const graph_t *graph, graph_t *g2);
void Graph_addEdge(graph_t *graph, unsigned char n1, unsigned char n2);
bool Graph_hasEdge(const graph_t *graph, unsigned char n1, unsigned char n2);
void Graph_removeEdge(graph_t *graph, unsigned char n1, unsigned char n2);
adjlist_t * Graph_neighbors(graph_t *graph, unsigned char n);


#endif
