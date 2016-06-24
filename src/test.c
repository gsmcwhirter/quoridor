#include <stdio.h>
#include <stdbool.h>

#include "timer/timer.h"
#include "term/term.h"

#include "board.h"
#include "gamestate.h"
#include "search.h"

int
main()
{
  board_t *board = Board_create();
  gamestate_t *state = GameState_create(board, PLAYER1);

  GameState_print(state, PLAYER1);

  pathinfo_t *path;

  timer_t timer;

  searchresult_t * startres1 = SearchResult_createWithSize(1, 0, 1);
  timer_start(&timer);
  Search_bfs_all(startres1, state->board, PLAYER1, state->board->player1);
  timer_pause(&timer);

  printf("Delta (us): %i\n", timer_delta_us(&timer));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer));

  printf("Shortest Paths: %i (max: %i)\n", startres1->count, startres1->results_desired);
  printf("Shortest Path Length: %i\n", startres1->shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < startres1->count; i++)
  {
    path = *(startres1->shortest_paths + i);
    printf("\t");
    PathInfo_print(path);
  }

  SearchResult_destroy(startres1, true);

  searchresult_t * startres2 = SearchResult_createWithSize(1, 0, 1);
  timer_unpause(&timer);
  Search_bfs_all(startres2, state->board, PLAYER2, state->board->player2);
  timer_pause(&timer);

  printf("Delta (us): %i\n", timer_delta_us(&timer));
	printf("Delta (ms): %li\n", timer_delta_ms(&timer));

  printf("Shortest Paths: %i (max: %i)\n", startres2->count, startres2->results_desired);
  printf("Shortest Path Length: %i\n", startres2->shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < startres2->count; i++)
  {
    path = *(startres2->shortest_paths + i);
    printf("\t");
    PathInfo_print(path);
  }

  SearchResult_destroy(startres2, true);

  Board_addWall(board, VERTICAL, 6, 'g');
  Board_addWall(board, HORIZONTAL, 3, 'e');
  Board_movePlayer(board, PLAYER1, 3, 'e');
  Board_movePlayer(board, PLAYER2, 6, 'g');

  GameState_print(state, PLAYER1);


  searchresult_t * res = SearchResult_create(0, 2);

  Search_bfs_all(res, state->board, PLAYER1, state->board->player1);

  printf("Shortest Paths: %i (max: %i)\n", res->count, res->results_desired);
  printf("Shortest Path Length: %i\n", res->shortest_length);
  printf("Shortest Paths:\n");
  for (int i = 0; i < res->count; i++)
  {
    path = *(res->shortest_paths + i);
    printf("\t");
    PathInfo_print(path);
  }

  SearchResult_destroy(res, true);
  GameState_destroy(state);

  return 0;
}
