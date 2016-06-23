#include <stdio.h>

#include "path.h"
#include "search.h"
#include "graph.h"
#include "board.h"
#include "gamestate.h"

searchresult_t *
SearchResult_create()
{
  return SearchResult_createWithSize(8);
}

searchresult_t *
SearchResult_createWithSize(int ct)
{
  searchresult_t *res = malloc(sizeof(searchresult_t));
  res->count = 0;
  res->max_count = ct;
  res->shortest_length = 82;
  res->shortest_paths = malloc(sizeof(pathinfo_t *) * res->max_count);

  return res;
}


bool
SearchResult_add(searchresult_t *res, pathinfo_t *path)
{
    if (path->length > res->shortest_length + BFS_MARGIN) return false;
    else if (path->length >= res->shortest_length)
    {
      if (res->count == res->max_count) SearchResult_expand(res);
      *(res->shortest_paths + res->count) = path;
      res->count++;
      return true;
    }
    else
    {
      res->shortest_length = path->length;
      for (int i = res->count-1; i >= 0; i--)
      {
        if ((*(res->shortest_paths + i))->length > res->shortest_length + BFS_MARGIN)
        {
          PathInfo_destroy(*(res->shortest_paths + i));
          res->count--;
          *(res->shortest_paths + i) = *(res->shortest_paths + res->count);
        }
      }
      *(res->shortest_paths + res->count) = path;
      res->count++;

      // printf("ADDED!\n");

      return true;
    }
}


void
SearchResult_expand(searchresult_t *res)
{
  res->max_count = res->max_count * 2;
  pathinfo_t **old = res->shortest_paths;

  res->shortest_paths = malloc(sizeof(pathinfo_t *) * res->max_count);
  for (int i = 0; i < res->count; i++)
  {
    *(res->shortest_paths + i) = *(old + i);
  }
  free(old);
}

void
SearchResult_destroy(searchresult_t *res, bool recurse)
{
  if (res != NULL)
  {
    if (recurse)
    {
      for (int i = 0; i < res->count; i++)
      {
        PathInfo_destroy(*(res->shortest_paths + i));
      }
    }
    free(res->shortest_paths);
    free(res);
  }
}

pathlist_t *
PathList_create(pathinfo_t *path)
{
  pathlist_t *pl = malloc(sizeof(pathlist_t));
  pl->path = path;
  pl->next = NULL;

  return pl;
}

void
PathList_destroy(pathlist_t *pl)
{
  if (pl != NULL)
  {
    PathInfo_destroy(pl->path);

    pathlist_t *plnext;
    while ((plnext = pl->next))
    {
      pl->next = NULL;
      PathList_destroy(plnext);
    }

    free(pl);
  }
}

pathqueue_t *
PathQueue_create()
{
  pathqueue_t * queue = malloc(sizeof(pathqueue_t));
  queue->head = NULL;
  queue->tail = NULL;

  return queue;
}

void
PathQueue_push(pathqueue_t *queue, pathlist_t *path)
{
  if (queue->head == NULL)
  {
    queue->head = path;
    queue->tail = path;
  }
  else
  {
    queue->tail->next = path;
    queue->tail = path;
  }
}

pathlist_t *
PathQueue_shift(pathqueue_t *queue)
{
  pathlist_t * ret;
  if (queue->head == NULL)
  {
    ret = NULL;
  }
  else
  {
    ret = queue->head;
    queue->head = ret->next;
    ret->next = NULL;

    if (queue->head == NULL)
    {
      queue->tail = NULL;
    }
  }

  return ret;
}


void
PathQueue_destroy(pathqueue_t *queue)
{
  if (queue != NULL)
  {
    PathList_destroy(queue->head);
    free(queue);
  }
}

searchresult_t *
Search_bfs_all(board_t *board, player_t player, int start)
{
  searchresult_t *res = SearchResult_create();
  pathqueue_t *queue = PathQueue_create();
  PathQueue_push(queue, PathList_create(PathInfo_create(start)));

  pathlist_t *curr;
  adjlist_t *neighbors;
  int curr_sq;
  pathinfo_t *to_add;
  int neighbor;

  // int ct = 0;

  while ((curr = PathQueue_shift(queue)))
  {
    // printf("here %i\n", ct);
    if (curr->path->length < res->shortest_length + BFS_MARGIN)
    {
      // printf("here2 %i\n", ct);
      // PathInfo_print(curr->path);
      curr_sq = Path_firstSquare(curr->path->path);
      // printf("here3 %i\n", ct);
      neighbors = Graph_neighbors(board->squares, curr_sq);
      // printf("here4 %i\n", ct);
      for (int i = 0; i < neighbors->degree; i++)
      {
        neighbor = *(neighbors->neighbors + i);
        // printf("neighbor: %i\n", neighbor);
        if (!PathInfo_visited(curr->path, neighbor))
        {
          // printf("not visited\n");
          to_add = PathInfo_clone(curr->path);
          // printf("cloned\n");
          PathInfo_unshift(to_add, neighbor);
          // printf("unshifted\n");

          if ((player == PLAYER1 && neighbor >= PLAYER1_TARGET) || (player == PLAYER2 && neighbor <= PLAYER2_TARGET))
          {
            // printf("REACHED THE END! (%i)\n", to_add->length);
            if (!SearchResult_add(res, to_add))
            {
              PathInfo_destroy(to_add);
            }
          }
          else if (to_add->length < res->shortest_length + BFS_MARGIN)
          {
            // printf("adding one %i\n", ct);
            PathQueue_push(queue, PathList_create(to_add));
          }
          else
          {
            PathInfo_destroy(to_add);
          }
        }
        // else
        // {
        //   printf("visited\n");
        // }
      }
    }
    PathList_destroy(curr);
  }

  PathQueue_destroy(queue);

  return res;
}

pathinfo_t * Search_bfs_one(board_t *board, player_t player, int start);
pathinfo_t * Search_dfs(board_t *board, player_t player, int start);
