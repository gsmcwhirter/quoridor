#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "path.h"

void
Path_init(path_t *path)
{
  path->length = 0;
}

path_t *
Path_clone(const path_t *path, path_t *new)
{
  if (path == NULL) return NULL;

  if (new == NULL)
    new = malloc(sizeof(path_t));

  new->length = path->length;
  for (int i = 0; i < path->length; i++)
  {
    new->square[i] = path->square[i];
  }

  return new;
}


void
Path_push(path_t *path, unsigned char square)
{
  path->square[path->length] = square;
  path->length++;
}


unsigned char
Path_pop(path_t *path)
{
  path->length--;
  return path->square[path->length];
}


unsigned char Path_current(path_t *path)
{
  return path->square[path->length - 1];
}


void Path_print(path_t *path)
{
  for (int i = 0; i < path->length; i++)
  {
    if (i == 0)
      printf(" [%i]", path->square[i]);
    else
      printf(" %i", path->square[i]);
  }
  printf("\n");
}
