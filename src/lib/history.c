#include <stdio.h>

#include "term/term.h"

#include "history.h"



gamehistory_t *
GameHistory_create()
{
  gamehistory_t *h = malloc(sizeof(gamehistory_t));
  h->prev = NULL;
  h->next = NULL;
  h->move = NULL;

  return h;
}

gamehistory_t *
GameHistory_clone(gamehistory_t *history)
{
  gamehistory_t *h = GameHistory_create();

  if (history->move != NULL)
    h->move = GameMove_clone(history->move);

  if (history->next != NULL)
  {
    h->next = GameHistory_clone(history->next);
    h->next->prev = h;
  }

  return h;
}

void
GameHistory_push(gamehistory_t *history, gamemove_t *move)
{
  gamehistory_t *tail = history;
  while (tail->next != NULL)
  {
    tail = tail->next;
  }

  tail->next = GameHistory_create();
  tail->next->prev = tail;
  tail->move = move;
}

gamehistory_t *
GameHistory_get(gamehistory_t *history, int i)
{
  if (i <= 0) return history;
  else if (history->next != NULL) return GameHistory_get(history->next, i-1);
  else return NULL;
}

gamehistory_t *
GameHistory_pop(gamehistory_t *history)
{
  gamehistory_t *tail = history;
  while (tail->next != NULL)
  {
    tail = tail->next;
  }

  if (tail->prev != NULL)
  {
    tail->prev->next = NULL;
    tail->move = tail->prev->move;
    tail->prev->move = NULL;
    return tail;
  }
  else
  {
    return NULL;
  }
}

void
GameHistory_print(gamehistory_t *history)
{
  printf("History: ");
  term_color("grey");
  gamehistory_t *curr = history;
  while (curr != NULL && curr->move != NULL)
  {
    GameMove_print(curr->move);
    if (curr->next != NULL && curr->next->move != NULL)
      printf(",");
    curr = curr->next;
  }
  term_reset();
  printf("\n");
}

void
GameHistory_destroy(gamehistory_t *history)
{
  if (history != NULL)
  {
    if (history->next != NULL)
    {
      GameHistory_destroy(history->next);
    }

    if (history->move != NULL)
    {
      GameMove_destroy(history->move);
    }

    if (history->prev != NULL)
    {
      history->prev->next = NULL;
    }

    free(history);
  }
}
