#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "path.h"

void
Path_init(path_t *path)
{
  path->size = 0;
}

path_t *
Path_clone(const path_t *path, path_t *new)
{
  if (path == NULL) return NULL;

  if (new == NULL)
    new = malloc(sizeof(path_t));

  new->size = path->size;
  for (int i = 0; i < path->size; i++)
  {
    new->square[i] = path->square[i];
  }

  return new;
}


// int
// Path_firstSquare(path_t *path)
// {
//   if (path->next == NULL)
//   {
//     return -1;
//   }
//   else
//   {
//     return path->next->square;
//   }
// }

void
Path_push(path_t *path, unsigned char square)
{
  // path_t *tail = path;
  // while (tail->next != NULL)
  // {
  //   tail = tail->next;
  // }
  // tail->next = Path_create();
  // tail->next->prev = tail;
  // tail->next->square = square;
  path->square[path->size] = square;
  path->size++;
}


unsigned char
Path_pop(path_t *path)
{
  // path_t *tail = path;
  // while (tail->next != NULL)
  // {
  //   tail = tail->next;
  // }
  //
  // int ret = tail->square;
  //
  // if (tail->prev != NULL)
  // {
  //   tail->prev->next = NULL;
  //   tail->prev = NULL;
  //   Path_destroy(tail);
  // }
  path->size--;
  return path->square[path->size];
}

// void
// Path_unshift(path_t *path, int square)
// {
//   // printf("%p\n", path);
//   // printf("%p\n", path->prev);
//   path_t* head = path;
//   if (head->prev == NULL && head->square == -1)
//   {
//     head = head->next;
//   }
//
//   if (head == NULL)
//   {
//     Path_push(path, square);
//   }
//   else
//   {
//     path_t *root = head->prev;
//     head->prev = Path_create();
//     head->prev->next = head;
//
//     head->prev->square = square;
//
//     head->prev->prev = root;
//     root->next = head->prev;
//   }
// }
//
// int
// Path_shift(path_t *root)
// {
//   path_t *head;
//   // if (root->prev == NULL && root->square == -1)
//   // {
//     head = root->next;
//   // }
//   // else
//   // {
//   //   head = root;
//   // }
//
//   if (head != NULL)
//   {
//     int ret = head->square;
//     root->next = head->next;
//     if (head->next != NULL)
//     {
//       head->next->prev = root;
//     }
//     head->next = NULL;
//     head->prev = NULL;
//     Path_destroy(head);
//     return ret;
//   }
//   else
//   {
//     return -1;
//   }
// }

void Path_destroy(path_t *path)
{
  //assert path->next == NULL and path->prev == NULL
  if (path != NULL)
  {
    // //path->prev = NULL;
    // Path_destroy(path->next);
    // //path->next = NULL;
    // free(path);
  }
}

void Path_print(path_t *path)
{
  // path_t *curr = path;
  // printf(" %i", curr->square);
  // while ((curr = curr->next))
  // {
  //   printf(" %i", curr->square);
  // }
  // if (path->next != NULL)
  //   Path_print(path->next);
  // if (path->square == -1)
  //   printf("\n");
  // else if (path->next == NULL)
  //   printf(" [%i]", path->square);
  // else
  //   printf(" %i", path->square);

  for (int i = 0; i < path->size; i++)
  {
    if (i == 0)
      printf(" [%i]", path->square[i]);
    else
      printf(" %i", path->square[i]);
  }
  printf("\n");
}

// pathinfo_t *
// PathInfo_create(int square, visitedrec_t *visited)
// {
//   pathinfo_t *pathinfo = malloc(sizeof(pathinfo_t));
//   pathinfo->length = 1;
//   pathinfo->path = Path_create();
//   Path_push(pathinfo->path, square);
//   pathinfo->visited = visited;
//
//   return pathinfo;
// }
//
// pathinfo_t *
// PathInfo_clone(pathinfo_t *path)
// {
//   pathinfo_t *new = malloc(sizeof(pathinfo_t));
//   new->length = path->length;
//   new->path = Path_clone(path->path);
//   // new->visited = calloc(SQUARES_SIZE_SQ, sizeof(bool));
//   // for (int i = 0; i < SQUARES_SIZE_SQ; i++)
//   // {
//   //   *(new->visited + i) = *(path->visited + i);
//   // }
//   new->visited = path->visited;
//
//   return new;
// }
//
// bool
// PathInfo_visited(pathinfo_t *pathinfo, int square)
// {
//   return VisitedRecord_visited(pathinfo->visited, square);
// }
//
// bool
// PathInfo_push(pathinfo_t *pathinfo, int square)
// {
//   if (PathInfo_visited(pathinfo, square)) return false;
//
//   pathinfo->length++;
//   Path_push(pathinfo->path, square);
//   VisitedRecord_setVisited(pathinfo->visited, square);
//
//   return true;
// }
//
//
// int
// PathInfo_pop(pathinfo_t *pathinfo)
// {
//   int ret = Path_pop(pathinfo->path);
//
//   if (ret >= 0)
//   {
//     pathinfo->length--;
//     // *(pathinfo->visited + ret) = false;
//   }
//
//   return ret;
// }
//
//
// bool
// PathInfo_unshift(pathinfo_t *pathinfo, int square)
// {
//   // if (PathInfo_visited(pathinfo, square)) return false; //speed up?
//
//   pathinfo->length++;
//   // printf("xxx\n");
//   Path_unshift(pathinfo->path, square);
//   // printf("yyy\n");
//   VisitedRecord_setVisited(pathinfo->visited, square);
//
//   // PathInfo_print(pathinfo);
//
//   return true;
// }
//
// int
// PathInfo_shift(pathinfo_t *pathinfo)
// {
//   int ret = Path_shift(pathinfo->path);
//
//   if (ret >= 0)
//   {
//     pathinfo->length--;
//     // *(pathinfo->visited + ret) = false;
//   }
//
//   return ret;
// }
//
// void
// PathInfo_destroy(pathinfo_t *pathinfo)
// {
//   if (pathinfo != NULL)
//   {
//     pathinfo->visited = NULL;
//     path_t *path;
//     while((path = pathinfo->path))
//     {
//       pathinfo->path = path->next;
//       path->next = NULL;
//       if (pathinfo->path != NULL)
//       {
//         pathinfo->path->prev = NULL;
//       }
//       Path_destroy(path);
//     }
//     free(pathinfo);
//   }
// }
//
// void
// PathInfo_print(pathinfo_t *pathinfo)
// {
//   printf("(Length %i):", pathinfo->length);
//   Path_print(pathinfo->path);
// }
