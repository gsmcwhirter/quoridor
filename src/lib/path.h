#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

#include "board.h"

typedef struct Path {
  int square;
  struct Path* prev;
  struct Path* next;
} path_t;

path_t * Path_create();
path_t * Path_clone(path_t *path);
int Path_firstSquare(path_t *path);
void Path_push(path_t *path, int square);
int Path_pop(path_t *path);
void Path_unshift(path_t *path, int square);
int Path_shift(path_t *path);
void Path_destroy(path_t *path);
void Path_print(path_t *path);

typedef struct VisitedRecord {
  bool *visited;
} visitedrec_t;

typedef struct PathInfo {
  int length;
  path_t *path;
  visitedrec_t *visited;
} pathinfo_t;

visitedrec_t * VisitedRecord_create();
bool VisitedRecord_visited(visitedrec_t *rec, int square);
void VisitedRecord_setVisited(visitedrec_t *rec, int square);
void VisitedRecord_destroy(visitedrec_t *rec);

pathinfo_t * PathInfo_create(int square, visitedrec_t *visited);
pathinfo_t * PathInfo_clone(pathinfo_t *path);
bool PathInfo_visited(pathinfo_t *pathinfo, int square);
bool PathInfo_push(pathinfo_t *pathinfo, int square);
int PathInfo_pop(pathinfo_t *pathinfo);
bool PathInfo_unshift(pathinfo_t *pathinfo, int square);
int PathInfo_shift(pathinfo_t *pathinfo);
void PathInfo_destroy(pathinfo_t *pathinfo);
void PathInfo_print(pathinfo_t *pathinfo);

#endif
