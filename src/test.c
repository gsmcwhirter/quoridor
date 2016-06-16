#include <stdio.h>
#include "term.h"
#include "board.h"
#include "gamestate.h"
#include "term/term.h"

int
main()
{
  board_t *board = Board_create();
  Board_print(board);

  gamestate_t *state = GameState_create(board, true);
  term_color("grey");
  printf("Turn of player: %i\n", (1-state->player1_turn) + 1);
  printf("Player 1 Walls: %i\n", state->player1_walls);
  printf("Player 2 Walls: %i\n", state->player2_walls);
  exit(0);
}
