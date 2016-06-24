#include <stdio.h>
#include <stdbool.h>
#include "term.h"
#include "board.h"
#include "gamestate.h"
#include "term/term.h"
#include "search.h"

int
main()
{
  board_t *board = Board_create();
  gamestate_t *state = GameState_create(board, PLAYER1);

  GameState_print(state, PLAYER1);

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
  pathinfo_t *path;
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
