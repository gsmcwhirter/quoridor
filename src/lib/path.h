#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

#include "board.h"

typedef struct Path {
  int length;
  unsigned char square[81];
} path_t;

void Path_init(path_t *path);
path_t * Path_clone(const path_t *path, path_t *new);
void Path_push(path_t *path, unsigned char square);
unsigned char Path_pop(path_t *path);
unsigned char Path_current(path_t *path);
void Path_print(path_t *path);

#endif
