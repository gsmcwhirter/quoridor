#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>

#include "graph.h"
#include "board.h"
#include "path.h"

typedef struct SearchResult {
  int count;
  int max_count;
  int shortest_length;
  pathinfo_t **shortest_paths;
} searchresult_t;

searchresult_t * SearchResult_create();
searchresult_t * SearchResult_createWithSize(int ct);
bool SearchResult_add(searchresult_t *res, pathinfo_t *path);
void SearchResult_expand(searchresult_t *res);
void SearchResult_destroy(searchresult_t *res, bool recurse);

typedef struct PathList {
  pathinfo_t *path;
  struct PathList *next;
} pathlist_t;

pathlist_t * PathList_create(pathinfo_t *path);
void PathList_destroy(pathlist_t *pl);

typedef struct PathInfoQueue {
  pathlist_t *head;
  pathlist_t *tail;
} pathqueue_t;

pathqueue_t * PathQueue_create();
void PathQueue_push(pathqueue_t *queue, pathlist_t *path);
pathlist_t * PathQueue_shift(pathqueue_t *queue);
void PathQueue_destroy(pathqueue_t *queue);

searchresult_t * Search_bfs_all(board_t *board, player_t player, int start);
pathinfo_t * Search_bfs_one(board_t *board, player_t player, int start);
pathinfo_t * Search_dfs(board_t *board, player_t player, int start);


#endif
