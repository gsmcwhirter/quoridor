#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>

#include "graph.h"
#include "board.h"
#include "path.h"

typedef struct SearchResult {
  int count;
  int results_desired;
  int bfs_margin;
  int max_count;
  int shortest_length;
  bool length_only;
  path_t **shortest_paths;
} searchresult_t;

searchresult_t * SearchResult_create(int results, int bfs_margin);
searchresult_t * SearchResult_createWithSize(int results, int bfs_margin, int ct);
bool SearchResult_add(searchresult_t *res, path_t *path);
void SearchResult_expand(searchresult_t *res);
void SearchResult_reset(searchresult_t *res, bool destroy);
void SearchResult_destroy(searchresult_t *res, bool recurse);

typedef struct PathList {
  path_t *path;
  struct PathList *next;
} pathlist_t;

pathlist_t * PathList_create(path_t *path);
void PathList_destroy(pathlist_t *pl);

typedef struct PathInfoQueue {
  pathlist_t *head;
  pathlist_t *tail;
} pathqueue_t;

pathqueue_t * PathQueue_create();
void PathQueue_push(pathqueue_t *queue, pathlist_t *path);
pathlist_t * PathQueue_shift(pathqueue_t *queue);
void PathQueue_destroy(pathqueue_t *queue);
//
// typedef struct IntQueueItem {
//   int val;
//   int dist;
//   struct IntQueueItem* next;
// } iqitem_t;
//
// iqitem_t * IQItem_create(int i, int d);
// void IQItem_destroy(iqitem_t *i);
//
// typedef struct IntQueue {
//   iqitem_t *head;
//   iqitem_t *tail;
// } intqueue_t;
//
// intqueue_t * IntQueue_create();
// void IntQueue_push(intqueue_t *q, iqitem_t *item);
// iqitem_t * IntQueue_shift(intqueue_t *iq);
// void IntQueue_destroy(intqueue_t *iq);

void Search_bfs_all(searchresult_t *res, board_t *board, player_t player, int start);
void Search_bfs_all_lengthOnly(searchresult_t *res, board_t *board, player_t player, int start);
// pathinfo_t * Search_dfs(board_t *board, player_t player, int start);


#endif
