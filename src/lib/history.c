#include <stdio.h>

#include "term/term.h"

#include "gamestate.h"
#include "history.h"


void
GameHistory_init(gamehistory_t *h, const gamestate_t *state)
{
  h->prev = NULL;
  h->next = NULL;
  h->move = NULL;
  GameState_clone(state, &(h->state));
}

gamehistory_t *
GameHistory_clone(const gamehistory_t *history, gamehistory_t *h)
{
  if (history == NULL)
  {
    return NULL;
  }

  if (h == NULL)
  {
    h = malloc(sizeof(gamehistory_t));
    GameHistory_init(h, &(history->state));
  }

  GameMove_clone(history->move, h->move);

  if (history->next != NULL)
  {
    h->next = GameHistory_clone(history->next, NULL);
    h->next->prev = h;
  }

  return h;
}

void
GameHistory_push(gamehistory_t *history, gamemove_t *move, gamestate_t *state)
{
  gamehistory_t *tail = history;
  while (tail->next != NULL)
  {
    tail = tail->next;
  }

  tail->next = malloc(sizeof(gamehistory_t));
  GameHistory_init(tail->next, state);
  tail->next->prev = tail;
  GameMove_clone(move, tail->move);
  //GameState_clone(state, &(tail->state));
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
    GameState_clone(&(tail->prev->state), &(tail->state));
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
      free(history->move);
    }

    if (history->prev != NULL)
    {
      history->prev->next = NULL;
    }

    free(history);
  }
}
