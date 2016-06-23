#include <stdio.h>
#include "history.h"
#include "term/term.h"

gamemove_t *
GameMove_create(player_t player, int row, char col, walldir_t wall)
{
  gamemove_t *move = malloc(sizeof(gamemove_t));
  move->player = player;
  move->row = row;
  move->col = col;
  move->wall = wall;

  return move;
}

void
GameMove_print(gamemove_t *move)
{
  printf("%c%i", move->col, move->row);
  if (move->wall == HORIZONTAL)
  {
    printf("h");
  }
  else if (move->wall == VERTICAL)
  {
    printf("v");
  }
}

void
GameMove_destroy(gamemove_t *move)
{
  if (move != NULL)
  {
    free(move);
  }
}

gamehistory_t *
GameHistory_create()
{
  gamehistory_t *h = malloc(sizeof(gamehistory_t));
  h->prev = NULL;
  h->next = NULL;
  h->move = NULL;

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
  // assert tail->move == NULL;

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

  tail->prev->next = NULL;
  tail->move = tail->prev->move;
  tail->prev->move = NULL;

  return tail;
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
