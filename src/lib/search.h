#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>

#include "graph.h"
#include "board.h"
#include "path.h"

#define MAX_SEARCH_RESULTS 12

typedef struct SearchResult {
  unsigned char count;
  unsigned char results_desired;
  unsigned char bfs_margin;
  unsigned char max_count;
  unsigned char shortest_length;
  path_t shortest_paths[MAX_SEARCH_RESULTS];
} searchresult_t;

void SearchResult_init(searchresult_t *res, unsigned char results, unsigned char bfs_margin);
bool SearchResult_add(searchresult_t *res, path_t *path);
void SearchResult_reset(searchresult_t *res);

void Search_bfs_all(searchresult_t *res, board_t *board, player_t player, unsigned char start);
void Search_bfs_exists(searchresult_t *res, board_t *board, player_t player, unsigned char start);

#endif
