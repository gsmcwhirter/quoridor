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
  // board_t board;
  // Board_init(&board);
  gamestate_t state;
  GameState_init(&state, NULL, PLAYER1);

  GameState_print(&state, PLAYER1);

  // pathinfo_t *path;
  path_t *path;

  timer_t timer1;
  // timer_t timer2;
  timer_t timer3;
  // timer_t timer4;

  searchresult_t * startres1 = SearchResult_createWithSize(1, 0, 1);
  timer_start(&timer1);
  Search_bfs_all(startres1, &(state.board), PLAYER1, state.board.player1);
  timer_pause(&timer1);

  printf("Full player 1\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer1));
	printf("Delta (ms): %ld\n", timer_delta_ms(&timer1));

  printf("Shortest Paths: %i (max: %i)\n", startres1->count, startres1->results_desired);
  printf("Shortest Path Length: %i\n", startres1->shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < startres1->count; i++)
  {
    path = *(startres1->shortest_paths + i);
    printf("\t");
    Path_print(path);
  }

  SearchResult_destroy(startres1, true);

  // searchresult_t * startres3 = SearchResult_createWithSize(1, 0, 0);
  // timer_start(&timer2);
  // Search_bfs_all_lengthOnly(startres3, &(state.board), PLAYER1, state.board.player1);
  // timer_pause(&timer2);
  //
  // printf("Length player 1\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer2));
	// printf("Delta (ms): %ld\n", timer_delta_ms(&timer2));
  //
  // printf("Shortest Paths: %i (max: %i)\n", startres3->count, startres3->results_desired);
  // printf("Shortest Path Length: %i\n", startres3->shortest_length);
  // // printf("Shortest Paths:\n");
  // //
  // // for (int i = 0; i < startres1->count; i++)
  // // {
  // //   path = *(startres1->shortest_paths + i);
  // //   printf("\t");
  // //   PathInfo_print(path);
  // // }
  //
  // SearchResult_destroy(startres3, true);


  searchresult_t * startres2 = SearchResult_createWithSize(1, 0, 1);
  timer_start(&timer3);
  Search_bfs_all(startres2, &(state.board), PLAYER2, state.board.player2);
  timer_pause(&timer3);

  printf("Full player 2\n");
  printf("Delta (us): %i\n", timer_delta_us(&timer3));
	printf("Delta (ms): %li\n", timer_delta_ms(&timer3));

  printf("Shortest Paths: %i (max: %i)\n", startres2->count, startres2->results_desired);
  printf("Shortest Path Length: %i\n", startres2->shortest_length);
  printf("Shortest Paths:\n");

  for (int i = 0; i < startres2->count; i++)
  {
    path = *(startres2->shortest_paths + i);
    printf("\t");
    Path_print(path);
  }

  SearchResult_destroy(startres2, true);

  // searchresult_t * startres4 = SearchResult_createWithSize(1, 0, 0);
  // timer_start(&timer4);
  // Search_bfs_all_lengthOnly(startres4, &(state.board), PLAYER2, state.board.player2);
  // timer_pause(&timer4);
  //
  // printf("Length player 2\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer4));
	// printf("Delta (ms): %li\n", timer_delta_ms(&timer4));
  //
  // printf("Shortest Paths: %i (max: %i)\n", startres4->count, startres4->results_desired);
  // printf("Shortest Path Length: %i\n", startres4->shortest_length);
  // // printf("Shortest Paths:\n");
  // //
  // // for (int i = 0; i < startres2->count; i++)
  // // {
  // //   path = *(startres2->shortest_paths + i);
  // //   printf("\t");
  // //   PathInfo_print(path);
  // // }
  //
  // SearchResult_destroy(startres4, true);

  Board_addWall(&(state.board), VERTICAL, 6, 'g');
  Board_addWall(&(state.board), HORIZONTAL, 3, 'e');
  Board_movePlayer(&(state.board), PLAYER1, 3, 'e');
  Board_movePlayer(&(state.board), PLAYER2, 6, 'g');

  GameState_print(&state, PLAYER1);


  searchresult_t * res = SearchResult_create(0, 2);

  Search_bfs_all(res, &(state.board), PLAYER1, state.board.player1);

  printf("Shortest Paths: %i (max: %i)\n", res->count, res->results_desired);
  printf("Shortest Path Length: %i\n", res->shortest_length);
  printf("Shortest Paths:\n");
  for (int i = 0; i < res->count; i++)
  {
    path = *(res->shortest_paths + i);
    printf("\t");
    Path_print(path);
  }

  SearchResult_destroy(res, true);
  GameState_destroy(&state);

  return 0;
}
