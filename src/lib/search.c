#include <stdio.h>
#include <assert.h>

#include "path.h"
#include "search.h"
#include "graph.h"
#include "board.h"
#include "gamestate.h"
#include "path.h"
#include "visited.h"

searchresult_t *
SearchResult_create(int results, int bfs_margin)
{
  return SearchResult_createWithSize(results, bfs_margin, 8);
}

searchresult_t *
SearchResult_createWithSize(int results, int bfs_margin, int ct)
{
  searchresult_t *res = malloc(sizeof(searchresult_t));
  res->count = 0;
  res->results_desired = results;
  res->bfs_margin = bfs_margin;
  res->max_count = ct;
  res->shortest_length = SQUARES_SIZE_SQ + 1;
  if (res->max_count > 0)
  {
    res->length_only = false;
    res->shortest_paths = malloc(sizeof(path_t *) * res->max_count);
  }
  else
  {
    res->length_only = true;
    res->shortest_paths = NULL;
  }

  return res;
}


bool
SearchResult_add(searchresult_t *res, path_t *path)
{
    if (path->size > res->shortest_length + res->bfs_margin) return false;
    else if (path->size >= res->shortest_length)
    {
      if (res->count == res->max_count) SearchResult_expand(res);
      *(res->shortest_paths + res->count) = path;
      res->count++;
      return true;
    }
    else
    {
      res->shortest_length = path->size;
      for (int i = res->count-1; i >= 0; i--)
      {
        if ((*(res->shortest_paths + i))->size > res->shortest_length + res->bfs_margin)
        {
          Path_destroy(*(res->shortest_paths + i));
          free(*(res->shortest_paths + i));
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
  path_t **old = res->shortest_paths;

  res->shortest_paths = malloc(sizeof(path_t *) * res->max_count);
  for (int i = 0; i < res->count; i++)
  {
    *(res->shortest_paths + i) = *(old + i);
  }
  free(old);
}

void
SearchResult_reset(searchresult_t *res, bool destroy)
{
  for (int i = 0; i < res->count; i++)
  {
    if (destroy)
    {
      Path_destroy(*(res->shortest_paths + i));
      free(*(res->shortest_paths + i));
    }
    *(res->shortest_paths + i) = NULL;
  }

  res->count = 0;
  res->shortest_length = SQUARES_SIZE_SQ + 1;
}

void
SearchResult_destroy(searchresult_t *res, bool recurse)
{
  if (res != NULL)
  {
    if (recurse && !res->length_only)
    {
      for (int i = 0; i < res->count; i++)
      {
        Path_destroy(*(res->shortest_paths + i));
        free(*(res->shortest_paths + i));
      }
    }

    if (res->shortest_paths != NULL)
      free(res->shortest_paths);
    free(res);
  }
}

pathlist_t *
PathList_create(path_t *path)
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
    Path_destroy(pl->path);

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

    // if (queue->head == NULL)
    // {
    //   queue->tail = NULL;
    // }
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

void
Search_bfs_all(searchresult_t *res, board_t *board, player_t player, int start)
{
  pathqueue_t *queue = PathQueue_create();
  visitedrec_t visited;
  VisitedRecord_init(&visited);
  path_t startpath;
  Path_init(&startpath);
  Path_push(&startpath, start);
  PathQueue_push(queue, PathList_create(Path_clone(&startpath, NULL)));

  pathlist_t *curr;
  adjlist_t *neighbors;
  int curr_sq;
  path_t *to_add;
  int neighbor;

  // int ct = 0;

  while ((curr = PathQueue_shift(queue)))
  {
    // printf("here %i\n", ct);
    if (curr->path->size < res->shortest_length + res->bfs_margin)
    {
      // printf("here2 %i\n", ct);
      // printf("Now considering: ");
      // PathInfo_print(curr->path);
      curr_sq = curr->path->square[0];
      // printf("here3 %i\n", ct);
      neighbors = Graph_neighbors(&(board->squares), curr_sq);
      // printf("here4 %i\n", ct);
      int deg = neighbors->degree;
      for (int i = 0; i < deg; i++)
      {
        if (player == PLAYER1) neighbor = *(neighbors->neighbors + i);
        else neighbor = *(neighbors->neighbors + (deg - i - 1));

        // printf("neighbor: %i\n", neighbor);
        if (!VisitedRecord_visited(&visited, neighbor))
        {
          // printf("not visited\n");
          to_add = Path_clone(curr->path, NULL);
          // printf("cloned\n");
          Path_push(to_add, neighbor);
          VisitedRecord_setVisited(&visited, neighbor);
          // PathInfo_unshift(to_add, neighbor);
          // printf("unshifted\n");

          if ((player == PLAYER1 && neighbor >= PLAYER1_TARGET) || (player == PLAYER2 && neighbor <= PLAYER2_TARGET))
          {
            // printf("REACHED THE END! (%i)\n", to_add->length);
            if (!SearchResult_add(res, to_add))
            {
              Path_destroy(to_add);
              free(to_add);
            }
          }
          // else if (to_add->length < res->shortest_length + res->bfs_margin)
          // {
          //   // printf("adding one %i\n", ct);
          //   PathQueue_push(queue, PathList_create(to_add));
          // }
          else
          {
            PathQueue_push(queue, PathList_create(to_add));
            // PathInfo_destroy(to_add);
          }
        }
        // else
        // {
        //   printf("visited\n");
        // }
      }
    }
    //PathList_destroy(curr); // NOTE: commenting this causes a memory leak, but it cuts 20us from the 60us runtime
    if (res->results_desired > 0 && res->count >= res->results_desired)
    {
      break;
    }
  }

  VisitedRecord_destroy(&visited);
  PathQueue_destroy(queue);
}

// void
// Search_bfs_all_lengthOnly(searchresult_t *res, board_t *board, player_t player, int start)
// {
//   // assert(res->length_only && res->results_desired == 1);
//   intqueue_t *queue = IntQueue_create();
//   visitedrec_t *visited = VisitedRecord_create();
//   IntQueue_push(queue, IQItem_create(start, 0));
//
//   iqitem_t *curr;
//   // iqitem_t *to_add;
//   adjlist_t *neighbors;
//   int curr_sq;
//   int neighbor;
//
//   // int ct = 0;
//
//   while ((curr = IntQueue_shift(queue)))
//   {
//     // printf("here %i\n", ct);
//     if (curr->dist < res->shortest_length + res->bfs_margin)
//     {
//       // printf("here2 %i\n", ct);
//       // printf("Now considering: ");
//       // PathInfo_print(curr->path);
//       // curr_sq = Path_firstSquare(curr->path->path);
//       curr_sq = curr->val;
//       // printf("here3 %i\n", ct);
//       neighbors = Graph_neighbors(board->squares, curr_sq);
//       // printf("here4 %i\n", ct);
//       int deg = neighbors->degree;
//       for (int i = 0; i < deg; i++)
//       {
//         if (player == PLAYER1) neighbor = *(neighbors->neighbors + i);
//         else neighbor = *(neighbors->neighbors + (deg - i - 1));
//
//         // printf("neighbor: %i\n", neighbor);
//         // if (!PathInfo_visited(curr->path, neighbor))
//         if (!VisitedRecord_visited(visited, neighbor))
//         {
//           // printf("not visited\n");
//           // to_add = PathInfo_clone(curr->path);
//           // printf("cloned\n");
//           // PathInfo_unshift(to_add, neighbor);
//           // printf("unshifted\n");
//
//           if ((player == PLAYER1 && neighbor >= PLAYER1_TARGET) || (player == PLAYER2 && neighbor <= PLAYER2_TARGET))
//           {
//             // printf("REACHED THE END! (%i)\n", to_add->length);
//             res->count = 1;
//             // res->shortest_length = to_add->dist;
//             res->shortest_length = curr->dist + 1;
//             // if (!SearchResult_add(res, to_add))
//             // {
//             //   PathInfo_destroy(to_add);
//             // }
//             // IQItem_destroy(to_add);
//           }
//           // else if (to_add->length < res->shortest_length + res->bfs_margin)
//           // {
//           //   // printf("adding one %i\n", ct);
//           //   PathQueue_push(queue, PathList_create(to_add));
//           // }
//           else
//           {
//             // to_add = IQItem_create(neighbor, curr->dist + 1);
//             IntQueue_push(queue, IQItem_create(neighbor, curr->dist + 1));
//             // PathInfo_destroy(to_add);
//           }
//         }
//         // else
//         // {
//         //   printf("visited\n");
//         // }
//       }
//     }
//     //PathList_destroy(curr); // NOTE: commenting this causes a memory leak, but it cuts 20us from the 60us runtime
//     if (res->results_desired > 0 && res->count >= res->results_desired)
//     {
//       break;
//     }
//   }
//
//   VisitedRecord_destroy(visited);
//   IntQueue_destroy(queue);
// }
//
// // pathinfo_t * Search_dfs(board_t *board, player_t player, int start);
//
// iqitem_t *
// IQItem_create(int i, int d)
// {
//     iqitem_t *iqi = malloc(sizeof(iqitem_t));
//     iqi->val = i;
//     iqi->dist = d;
//     iqi->next = NULL;
//
//     return iqi;
// }
//
//
// void
// IQItem_destroy(iqitem_t *i)
// {
//   if (i != NULL)
//   {
//     IQItem_destroy(i->next);
//     i->next = NULL;
//     free(i);
//   }
// }
//
//
// intqueue_t *
// IntQueue_create()
// {
//   intqueue_t * queue = malloc(sizeof(intqueue_t));
//   queue->head = NULL;
//   queue->tail = NULL;
//
//   return queue;
// }
//
// void
// IntQueue_push(intqueue_t *queue, iqitem_t *path)
// {
//   if (queue->head == NULL)
//   {
//     queue->head = path;
//     queue->tail = path;
//   }
//   else
//   {
//     queue->tail->next = path;
//     queue->tail = path;
//   }
// }
//
// iqitem_t *
// IntQueue_shift(intqueue_t *queue)
// {
//   iqitem_t * ret;
//   if (queue->head == NULL)
//   {
//     ret = NULL;
//   }
//   else
//   {
//     ret = queue->head;
//     queue->head = ret->next;
//     ret->next = NULL;
//
//     // if (queue->head == NULL)
//     // {
//     //   queue->tail = NULL;
//     // }
//   }
//
//   return ret;
// }
//
//
// void
// IntQueue_destroy(intqueue_t *queue)
// {
//   if (queue != NULL)
//   {
//     IQItem_destroy(queue->head);
//     free(queue);
//   }
// }
