#include <stdio.h>
#include <stdbool.h>

#include "timer/timer.h"
#include "term/term.h"

#include "board.h"
#include "gamestate.h"
#include "search.h"
#include "history.h"
#include "ai.h"

#define ITERATIONS 1000000l

int
main()
{
  gamestate_t state;
  GameState_init(&state, NULL, PLAYER1);

  GameState_print(&state, PLAYER1);
  // timer_t timer1;
  // timer_t timer2;
  // timer_t timer3;
  // timer_t timer4;
  //
  // searchresult_t res;
  //
  // #ifdef SEVERAL
  //   printf("RUNNING SEVERAL TESTS (%li)\n\n", ITERATIONS);
  // #endif
  //
  // /// Player 1 Full
  // SearchResult_init(&res, 1, 0);
  // timer_start(&timer1);
  // #ifdef SEVERAL
  // for (long i = 0; i < ITERATIONS; i++)
  // {
  //   SearchResult_reset(&res);
  // #endif
  //   Search_bfs_all(&res, &(state.board), PLAYER1, state.board.player1);
  // #ifdef SEVERAL
  // }
  // #endif
  // timer_pause(&timer1);
  //
  // printf("Full player 1\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer1));
	// printf("Delta (ms): %ld\n", timer_delta_ms(&timer1));
  //
  // printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  // printf("Shortest Path Length: %i\n", res.shortest_length);
  // printf("Shortest Paths:\n");
  //
  // for (int i = 0; i < res.count; i++)
  // {
  //   printf("\t");
  //   Path_print(&(res.shortest_paths[i]));
  // }
  // /// End Player 1 Full
  //
  // /// Player 1 Exists
  // SearchResult_init(&res, 1, 0);
  // timer_start(&timer2);
  // #ifdef SEVERAL
  // for (long i = 0; i < ITERATIONS; i++)
  // {
  //   SearchResult_reset(&res);
  // #endif
  //   Search_bfs_exists(&res, &(state.board), PLAYER1, state.board.player1);
  // #ifdef SEVERAL
  // }
  // #endif
  // timer_pause(&timer2);
  //
  // printf("Exists player 1\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer2));
	// printf("Delta (ms): %ld\n", timer_delta_ms(&timer2));
  //
  // printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  // printf("Shortest Path Length: %i\n", res.shortest_length);
  // /// End Player 1 Exists
  //
  // /// Player 2 Full
  // SearchResult_init(&res, 1, 0);
  // timer_start(&timer3);
  // #ifdef SEVERAL
  // for (long i = 0; i < ITERATIONS; i++)
  // {
  //   SearchResult_reset(&res);
  // #endif
  //   Search_bfs_all(&res, &(state.board), PLAYER2, state.board.player2);
  // #ifdef SEVERAL
  // }
  // #endif
  // timer_pause(&timer3);
  //
  // printf("Full player 2\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer1));
	// printf("Delta (ms): %ld\n", timer_delta_ms(&timer1));
  //
  // printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  // printf("Shortest Path Length: %i\n", res.shortest_length);
  // printf("Shortest Paths:\n");
  //
  // for (int i = 0; i < res.count; i++)
  // {
  //   printf("\t");
  //   Path_print(&(res.shortest_paths[i]));
  // }
  // /// End Player 2 Full
  //
  // /// Player 2 Exists
  // SearchResult_init(&res, 1, 0);
  // timer_start(&timer4);
  // #ifdef SEVERAL
  // for (long i = 0; i < ITERATIONS; i++)
  // {
  //   SearchResult_reset(&res);
  // #endif
  //   Search_bfs_exists(&res, &(state.board), PLAYER2, state.board.player2);
  // #ifdef SEVERAL
  // }
  // #endif
  // timer_pause(&timer4);
  //
  // printf("Exists player 2\n");
  // printf("Delta (us): %i\n", timer_delta_us(&timer4));
	// printf("Delta (ms): %ld\n", timer_delta_ms(&timer4));
  //
  // printf("Shortest Paths: %i (max: %i)\n", res.count, res.results_desired);
  // printf("Shortest Path Length: %i\n", res.shortest_length);
  // /// End Player 2 Exists


  /// Test AI
  #ifdef DEBUG
    printf("Creating move sequence...\n");
  #endif
  moveseq_t ms;
  MoveSequence_init(&ms);
  #ifdef DEBUG
    printf("done.\n");
  #endif

  #ifdef DEBUG
    printf("Creating history...\n");
  #endif
  gamehistory_t history;
  GameHistory_init(&history, &state);
  GameHistory_print(&history);
  #ifdef DEBUG
    printf("done creating history.\n");
  #endif

  #ifdef DEBUGAI
    printf("Creating AI Stage...\n");
  #endif
  aistage_t ai;
  AIStage_init(&ai, &state, &history);
  #ifdef DEBUGAI
    printf("generating possible moves...\n");
  #endif
  AIStage_generatePossibleMoves(&ai);
  #ifdef DEBUGAI
    printf("done setup.\n");
  #endif

  PossibleMoves_print(&(ai.nextmoves));

  bestmoves_t bm;

  GameHistory_print(&(ai.history));

  bm = AIStage_bestMoves(&ai, &ms, 3);

  BestMoves_print(&bm);

  /// End Test AI


  // /// Test Moves etc.
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
  // /// End Test Moves etc.

  return 0;
}
