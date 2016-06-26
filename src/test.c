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
  gamestate_t state;
  GameState_init(&state, NULL, PLAYER1);

  GameState_print(&state, PLAYER1);
  timer_t timer1;
  timer_t timer2;
  timer_t timer3;
  timer_t timer4;

  searchresult_t res;
  SearchResult_init(&res, 1, 0);
  timer_start(&timer1);
  Search_bfs_all(&res, &(state.board), PLAYER1, state.board.player1);
  timer_pause(&timer1);

  printf("Full player 1\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer1));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer1));

  printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  printf("Shortest Path Length: %i\n", res.shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < res.count; i++)
  {
    printf("\t");
    Path_print(&(res.shortest_paths[i]));
  }

  SearchResult_init(&res, 1, 0);
  timer_start(&timer2);
  Search_bfs_exists(&res, &(state.board), PLAYER1, state.board.player1);
  timer_pause(&timer2);

  printf("Exists player 1\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer2));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer2));

  printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  printf("Shortest Path Length: %i\n", res.shortest_length);

  SearchResult_init(&res, 1, 0);
  timer_start(&timer3);
  Search_bfs_all(&res, &(state.board), PLAYER2, state.board.player2);
  timer_pause(&timer3);

  printf("Full player 2\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer1));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer1));

  printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  printf("Shortest Path Length: %i\n", res.shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < res.count; i++)
  {
    printf("\t");
    Path_print(&(res.shortest_paths[i]));
  }

  SearchResult_init(&res, 1, 0);
  timer_start(&timer4);
  Search_bfs_exists(&res, &(state.board), PLAYER2, state.board.player2);
  timer_pause(&timer4);

  printf("Exists player 2\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer4));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer4));

  printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  printf("Shortest Path Length: %i\n", res.shortest_length);

  // Board_addWall(&(state.board), VERTICAL, 6, 'g');
  // Board_addWall(&(state.board), HORIZONTAL, 3, 'e');
  // Board_movePlayer(&(state.board), PLAYER1, 3, 'e');
  // Board_movePlayer(&(state.board), PLAYER2, 6, 'g');
  //
  // GameState_print(&state, PLAYER1);
  //
  // SearchResult_init(&res, 0, 2);
  // Search_bfs_all(&res, &(state.board), PLAYER1, state.board.player1);
  //
  // printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  // printf("Shortest Path Length: %i\n", res.shortest_length);
  // printf("Shortest Paths:\n");
  // for (int i = 0; i < res.count; i++)
  // {
  //   printf("\t");
  //   Path_print(&(res.shortest_paths[i]));
  // }

  return 0;
}
