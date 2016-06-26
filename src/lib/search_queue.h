#ifndef SEARCH_QUEUE_H
#define SEARCH_QUEUE_H

#include "board.h"
#include "path.h"

// this is 127 << 8
// #define LENGTH_MASK 32512ll
#define LENGTH_MASK 127

// this is 82 << 8
#define EMPTY_EXISTS_QUEUE_CODE 20992ll

typedef struct SearchQueue {
  unsigned char start;
  unsigned char length;
  path_t paths[SQUARES_SIZE_SQ];
} searchqueue_t;

void SearchQueue_init(searchqueue_t *q);
bool SearchQueue_push(searchqueue_t *queue, path_t *path);
bool SearchQueue_shift(searchqueue_t *queue, path_t *path);
void SearchQueue_print(searchqueue_t *q);

typedef struct ExistsQueue {
  unsigned char start;
  unsigned char length;
  unsigned char paths[SQUARES_SIZE_SQ];
} existsqueue_t;

void ExistsQueue_init(existsqueue_t *q);
bool ExistsQueue_push(existsqueue_t *queue, int loc);
unsigned char ExistsQueue_shift(existsqueue_t *queue);
void ExistsQueue_print(existsqueue_t *q);

#endif
