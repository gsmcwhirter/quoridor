#include <stdio.h>

#include "term/term.h"

#include "gamestate.h"
#include "history.h"
#include "move.h"


void
GameHistory_init(gamehistory_t *h, const gamestate_t *state)
{
  #ifdef DEBUG
    printf("Initializing history.");
  #endif
  h->prev = NULL;
  h->next = NULL;
  h->move_set = false;
  GameState_clone(state, &(h->state));
  #ifdef DEBUG
    printf("Initializing history done.\n");
  #endif
}

gamehistory_t *
GameHistory_clone(const gamehistory_t *history, gamehistory_t *h)
{
  #ifdef DEBUG
    printf("Inside cloning history.\n");
  #endif
  if (history == NULL)
  {
    #ifdef DEBUG
      printf("No original provided.\n");
    #endif
    return NULL;
  }

  if (h == NULL)
  {
    #ifdef DEBUG
      printf("No new history memory provided.\n");
    #endif
    h = malloc(sizeof(gamehistory_t));
    #ifdef DEBUG
      printf("Initializing new history.\n");
    #endif
    GameHistory_init(h, &(history->state));
  }

  if (history->move_set)
  {
    #ifdef DEBUGAI
      printf("Cloning game move.\n");
      GameMove_print(&(history->move));
      printf("\n");
    #endif
    GameMove_clone(&(history->move), &(h->move));
    h->move_set = true;
  }
  else
  {
    h->move_set = false;
  }

  if (history->next != NULL)
  {
    #ifdef DEBUGAI
      printf("Cloning next history item...\n");
    #endif
    h->next = GameHistory_clone(history->next, NULL);
    h->next->prev = h;
  }
  else
  {
    h->next = NULL;
  }

  #ifdef DEBUGAI
    printf("Cloning game state...\n");
  #endif
  GameState_clone(&(history->state), &(h->state));
  #ifdef DEBUGAI
    printf("done cloning state.\n");
  #endif

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
  GameMove_clone(move, &(tail->move));
  tail->move_set = true;
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
    tail->prev->move_set = false;
    return tail;
  }
  else
  {
    return NULL;
  }
}

gamehistory_t *
GameHistory_last(gamehistory_t *history)
{
  gamehistory_t *tail = history;
  while (tail->next != NULL)
  {
    tail = tail->next;
  }

  if (tail->prev != NULL)
  {
    return tail->prev;
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
  while (curr != NULL && curr->move_set)
  {
    GameMove_print(&(curr->move));
    if (curr->next != NULL && curr->next->move_set)
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

    if (history->prev != NULL)
    {
      history->prev->next = NULL;
    }

    free(history);
  }
}
