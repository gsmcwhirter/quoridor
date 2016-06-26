#include <stdbool.h>
#include <stdio.h>

#include "search_queue.h"
#include "path.h"

void
SearchQueue_init(searchqueue_t *q)
{
  q->start = 0;
  q->length = 0;
}

bool
SearchQueue_push(searchqueue_t *queue, path_t *path)
{
  #ifdef DEBUG
    printf("pushing onto queue with length %i...\n", queue->length);
  #endif
  if (queue->length >= SQUARES_SIZE_SQ)
  {
    #ifdef DEBUG
      printf("queue full!\n");
    #endif
    return false;
  }
  #ifdef DEBUG
    printf("saving to index %i...\n", (queue->start + queue->length) % SQUARES_SIZE_SQ);
  #endif
  Path_clone(path, &(queue->paths[(queue->start + queue->length) % SQUARES_SIZE_SQ]));
  #ifdef DEBUG
    printf("incrementing length...\n");
  #endif
  queue->length++;
  #ifdef DEBUG
    printf("queue items: %i\n", queue->length);
  #endif
  return true;
}

bool
SearchQueue_shift(searchqueue_t *queue, path_t *path)
{
  #ifdef DEBUG
    printf("trying to shift element off the queue...\n");
    printf("queue has start %i and length %i...\n", queue->start, queue->length);
  #endif
  if (queue->length == 0)
  {
    #ifdef DEBUG
      printf("no elements in the queue...\n");
    #endif
    return false;
  }

  #ifdef DEBUG
    printf("cloning path...\n");
  #endif
  Path_clone(&(queue->paths[queue->start]), path);
  #ifdef DEBUG
    printf("resetting start (was %i)...", queue->start);
  #endif
  queue->start = (queue->start + 1) % SQUARES_SIZE_SQ;
  #ifdef DEBUG
    printf("(now %i).\n", queue->start);
  #endif
  #ifdef DEBUG
    printf("reducing length (was %i)...", queue->length);
  #endif
  queue->length--;
  #ifdef DEBUG
    printf("(now %i).\n", queue->length);
  #endif
  return true;
}

void
SearchQueue_print(searchqueue_t *q)
{
  printf("Search queue (start %i, length %i):\n", q->start, q->length);
  int ind;
  for (int i = 0; i < q->length; i++)
  {
    ind = q->start + i;
    printf("\t");
    Path_print(&(q->paths[ind]));
  }
}

void
ExistsQueue_init(existsqueue_t *q)
{
  q->start = 0;
  q->length = 0;
}

bool
ExistsQueue_push(existsqueue_t *queue, int loc)
{
  #ifdef DEBUG
    printf("pushing onto queue with length %i...\n", queue->length);
  #endif
  if (queue->length >= SQUARES_SIZE_SQ)
  {
    #ifdef DEBUG
      printf("queue full!\n");
    #endif
    return false;
  }
  #ifdef DEBUG
    printf("saving to index %i...\n", (queue->start + queue->length) % SQUARES_SIZE_SQ);
  #endif
  queue->paths[(queue->start + queue->length) % SQUARES_SIZE_SQ] = loc;
  #ifdef DEBUG
    printf("incrementing length...\n");
  #endif
  queue->length++;
  #ifdef DEBUG
    printf("queue items: %i\n", queue->length);
  #endif
  return true;
}

unsigned char
ExistsQueue_shift(existsqueue_t *queue)
{
  unsigned char ret;
  #ifdef DEBUG
    printf("trying to shift element off the queue...\n");
    printf("queue has start %i and length %i...\n", queue->start, queue->length);
  #endif
  if (queue->length == 0)
  {
    #ifdef DEBUG
      printf("no elements in the queue...\n");
    #endif
    // return EMPTY_EXISTS_QUEUE_CODE;
    return SQUARES_SIZE_SQ;
  }

  // Path_clone(&(queue->paths[queue->start]), path);
  ret = queue->paths[queue->start];
  #ifdef DEBUG
    printf("resetting start (was %i)...", queue->start);
  #endif
  queue->start = (queue->start + 1) % SQUARES_SIZE_SQ;
  #ifdef DEBUG
    printf("(now %i).\n", queue->start);
  #endif
  #ifdef DEBUG
    printf("reducing length (was %i)...", queue->length);
  #endif
  queue->length--;
  #ifdef DEBUG
    printf("(now %i).\n", queue->length);
  #endif
  return ret;
}

void
ExistsQueue_print(existsqueue_t *q)
{
  printf("Exists queue (start %i, length %i):\n", q->start, q->length);
  int ind;
  printf(" ");
  for (int i = 0; i < q->length; i++)
  {
    ind = q->start + i;

    // printf(" %i,%i", q->paths[ind] & LENGTH_MASK, (q->paths[ind] & ~LENGTH_MASK)>>8);
    printf(" %i", q->paths[ind]);
  }
  printf("\n");
}
