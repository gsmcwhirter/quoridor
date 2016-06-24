#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct AdjList {
  int total_nodes;
  int degree;
  int max_degree;
  int* neighbors;
} adjlist_t;

typedef struct Graph {
  int nodes;
  int max_degree;
  int *node_names;
  adjlist_t **adj_lists;
} graph_t;

adjlist_t * AdjList_create(const int total_nodes);//, const int max_degree);
adjlist_t * AdjList_clone(adjlist_t *al);
void AdjList_addNeighbor(adjlist_t *al, int n);
void AdjList_sortNeighbors(adjlist_t *al);
bool AdjList_hasNeighbor(adjlist_t *al, int n);
int AdjList_findNeighbor(adjlist_t *al, int n);
void AdjList_removeNeighbor(adjlist_t *al, int n);
void AdjList_destroy(adjlist_t *al);
void AdjList_relabel(adjlist_t *al, int num_nodes, int *nodes);

graph_t * Graph_create(const int nodes, const int max_degree);
graph_t * Graph_clone(graph_t *graph);
graph_t * Graph_subgraph(graph_t *graph, int size, int *nodes);
void Graph_addEdge(graph_t *graph, int n1, int n2);
bool Graph_hasEdge(graph_t *graph, int n1, int n2);
void Graph_removeEdge(graph_t *graph, int n1, int n2);
void Graph_destroy(graph_t *graph);
adjlist_t * Graph_neighbors(graph_t *graph, int n);

#endif
