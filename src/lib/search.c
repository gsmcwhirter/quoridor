#include <stdio.h>
#include <assert.h>

#include "path.h"
#include "search.h"
#include "graph.h"
#include "board.h"
#include "gamestate.h"
#include "path.h"
#include "visited.h"
#include "search_queue.h"

// this is 127 << 8
#define LENGTH_MASK 32512


void
SearchResult_init(searchresult_t *res, unsigned char results, unsigned char bfs_margin)
{
  res->count = 0;
  res->results_desired = results;
  res->bfs_margin = bfs_margin;
  res->max_count = MAX_SEARCH_RESULTS;
  res->shortest_length = SQUARES_SIZE_SQ + 1;
}


bool
SearchResult_add(searchresult_t *res, path_t *path)
{
  if (res->count == res->max_count) return false;
  if (path->length > res->shortest_length + res->bfs_margin) return false;
  else if (path->length >= res->shortest_length)
  {
    Path_clone(path, &(res->shortest_paths[res->count]));
    res->count++;
    return true;
  }
  else
  {
    res->shortest_length = path->length;
    for (int i = res->count-1; i >= 0; i--)
    {
      if (res->shortest_paths[i].length > res->shortest_length + res->bfs_margin)
      {
        res->count--;
        Path_clone(&(res->shortest_paths[res->count]), &(res->shortest_paths[i]));
      }
    }
    Path_clone(path, &(res->shortest_paths[res->count]));
    res->count++;

    return true;
  }
}


void
SearchResult_reset(searchresult_t *res)
{
  res->count = 0;
  res->shortest_length = SQUARES_SIZE_SQ + 1;
}


void
Search_bfs_all(searchresult_t *res, board_t *board, player_t player, unsigned char start)
{
  #ifdef DEBUG
    printf("Initializing queue...\n");
  #endif
  searchqueue_t queue;
  SearchQueue_init(&queue);
  #ifdef DEBUG
    printf("Initializing visited...\n");
  #endif
  visitedrec_t visited;
  VisitedRecord_init(&visited);
  #ifdef DEBUG
    printf("Initializing path...\n");
  #endif
  path_t curr;
  Path_init(&curr);
  #ifdef DEBUG
    printf("Starting path...\n");
  #endif
  Path_push(&curr, start);

  #ifdef DEBUG
    printf("Starting queue...\n");
  #endif
  SearchQueue_push(&queue, &curr);

  #ifdef DEBUG
    SearchQueue_print(&queue);
  #endif

  adjlist_t *neighbors;
  unsigned char curr_sq;
  unsigned char neighbor;

  while (SearchQueue_shift(&queue, &curr))
  {
    #ifdef DEBUG
      printf("Loop iterating...\n");
      printf("now considering: ");
      Path_print(&curr);
    #endif
    #ifdef DEBUG
      SearchQueue_print(&queue);
    #endif
    if (curr.length < res->shortest_length + res->bfs_margin)
    {
      #ifdef DEBUG
        printf("path is short enough...\n");
        printf("getting current square...\n");
      #endif
      curr_sq = Path_current(&curr);
      #ifdef DEBUG
        printf("current square is %i.\n", curr_sq);
        printf("getting neighbors...\n");
      #endif
      neighbors = Graph_neighbors(&(board->squares), curr_sq);

      int deg = neighbors->degree;
      for (int i = 0; i < deg; i++)
      {
        #ifdef DEBUG
          printf("neighbors iteration...\n");
        #endif
        if (player == PLAYER1) neighbor = neighbors->neighbors[i];
        else neighbor = neighbors->neighbors[deg - i - 1];
        #ifdef DEBUG
          printf("considering neighbor %i...\n", neighbor);
        #endif

        if (!VisitedRecord_visited(&visited, neighbor))
        {
          #ifdef DEBUG
            printf("neighbor has not been visited...\n");
            printf("adding neighbor to current path...\n");
          #endif
          Path_push(&curr, neighbor);
          #ifdef DEBUG
            printf("current path: ");
            Path_print(&curr);
          #endif
          #ifdef DEBUG
            printf("setting neighbor as visited...\n");
          #endif
          VisitedRecord_setVisited(&visited, neighbor);

          #ifdef DEBUG
            printf("checking for path end...\n");
          #endif
          if ((player == PLAYER1 && neighbor >= PLAYER1_TARGET) || (player == PLAYER2 && neighbor <= PLAYER2_TARGET))
          {
            #ifdef DEBUG
              printf("reached the end. length=%i\n", curr.length);
              printf("adding path to the results...\n");
            #endif
            if (!SearchResult_add(res, &curr))
            {
              #ifdef DEBUG
                printf("could not add the path to results. too long?\n");
              #endif
            }
            #ifdef DEBUG
            else
              printf("path added to results.\n");
            #endif
          }
          else
          {
            #ifdef DEBUG
              printf("adding path to the queue...\n");
            #endif
            SearchQueue_push(&queue, &curr);
            #ifdef DEBUG
              printf("done.\n");
              printf("current queue length is %i\n", queue.length);
            #endif
          }
        }
        #ifdef DEBUG
        else
          printf("neighbor already visited.\n");
        #endif
      }
    }

    #ifdef DEBUG
      printf("do we have enough results?\n");
    #endif
    if (res->results_desired > 0 && res->count >= res->results_desired)
    {
      #ifdef DEBUG
        printf("found enough results. terminating search\n");
      #endif
      break;
    }
    #ifdef DEBUG
    else
      printf("not enough. continuing...\n");
    #endif
  }

  #ifdef DEBUG
    printf("finished loop.\n");
  #endif
}

void
Search_bfs_exists(searchresult_t *res, board_t *board, player_t player, unsigned char start)
{
  #ifdef DEBUG
    printf("Initializing queue...\n");
  #endif
  existsqueue_t queue;
  ExistsQueue_init(&queue);
  #ifdef DEBUG
    printf("Initializing visited...\n");
  #endif
  visitedrec_t visited;
  VisitedRecord_init(&visited);
  #ifdef DEBUG
    printf("Initializing path...\n");
  #endif
  int curr;
  #ifdef DEBUG
    printf("Starting path...\n");
  #endif
  curr = start;

  #ifdef DEBUG
    printf("Starting queue...\n");
  #endif
  ExistsQueue_push(&queue, curr);

  #ifdef DEBUG
    ExistsQueue_print(&queue);
  #endif

  adjlist_t *neighbors;
  unsigned char neighbor;
  int lbits = 82;
  unsigned char curr_sq;

  while ((curr = ExistsQueue_shift(&queue)) < SQUARES_SIZE_SQ)
  {
    lbits = curr & LENGTH_MASK;
    curr_sq = curr & ~LENGTH_MASK;
    #ifdef DEBUG
      printf("Loop iterating...\n");
      printf("now considering: %i\n", curr_sq);
    #endif
    #ifdef DEBUG
      ExistsQueue_print(&queue);
    #endif

    #ifdef DEBUG
      printf("getting neighbors...\n");
    #endif
    neighbors = Graph_neighbors(&(board->squares), curr_sq);

    int deg = neighbors->degree;
    for (int i = 0; i < deg; i++)
    {
      #ifdef DEBUG
        printf("neighbors iteration...\n");
      #endif
      if (player == PLAYER1) neighbor = neighbors->neighbors[i];
      else neighbor = neighbors->neighbors[deg - i - 1];
      #ifdef DEBUG
        printf("considering neighbor %i...\n", neighbor);
      #endif

      if (!VisitedRecord_visited(&visited, neighbor))
      {
        #ifdef DEBUG
          printf("neighbor has not been visited...\n");
        #endif
        #ifdef DEBUG
          printf("setting neighbor as visited...\n");
        #endif
        VisitedRecord_setVisited(&visited, neighbor);

        #ifdef DEBUG
          printf("checking for path end...\n");
        #endif
        if ((player == PLAYER1 && neighbor >= PLAYER1_TARGET) || (player == PLAYER2 && neighbor <= PLAYER2_TARGET))
        {
          res->count = 1;
          res->shortest_length = lbits >> 8;
          // #ifdef DEBUG
          //   printf("reached the end. length=%i\n", curr.length);
          //   printf("adding path to the results...\n");
          // #endif
          // if (!SearchResult_add(res, &curr))
          // {
          //   #ifdef DEBUG
          //     printf("could not add the path to results. too long?\n");
          //   #endif
          // }
          // #ifdef DEBUG
          // else
          //   printf("path added to results.\n");
          // #endif
        }
        else
        {
          #ifdef DEBUG
            printf("adding square to the queue...\n");
          #endif
          ExistsQueue_push(&queue, lbits + 128 + neighbor);
          #ifdef DEBUG
            printf("done.\n");
            printf("current queue length is %i\n", queue.length);
          #endif
        }
      }
      #ifdef DEBUG
      else
        printf("neighbor already visited.\n");
      #endif
    }


    #ifdef DEBUG
      printf("do we have enough results?\n");
    #endif
    if (res->count > 0)
    {
      #ifdef DEBUG
        printf("found enough results. terminating search\n");
      #endif
      break;
    }
    #ifdef DEBUG
    else
      printf("not enough. continuing...\n");
    #endif
  }

  #ifdef DEBUG
    printf("finished loop.\n");
  #endif
}
