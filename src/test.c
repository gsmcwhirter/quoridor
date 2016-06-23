#include <stdio.h>
#include <stdbool.h>
#include "term.h"
#include "board.h"
#include "gamestate.h"
#include "term/term.h"

int
main()
{
  board_t *board = Board_create();
  gamestate_t *state = GameState_create(board, PLAYER1);

  GameState_print(state);

  Board_addWall(board, VERTICAL, 6, 'g');
  Board_addWall(board, HORIZONTAL, 3, 'e');
  Board_movePlayer(board, PLAYER1, 3, 'e');
  Board_movePlayer(board, PLAYER2, 6, 'g');

  GameState_print(state);
  // printf("%i\n", Board_validWall(board, HORIZONTAL, 3, 7));
  // printf("%i\n", Board_validWall(board, VERTICAL, 7, 7));
  // printf("\n");
  // printf("%i\n", GameState_legalWall(state, PLAYER1, HORIZONTAL, 3, 7));
  // printf("%i\n", GameState_legalWall(state, PLAYER2, VERTICAL, 7, 7));
  // printf("\n");
  // printf("%i\n", adjacentSquares(3, 5, 4, 5));
  // printf("%i\n", adjacentSquares(3, 5, 3, 6));
  // printf("%i\n", adjacentSquares(3, 5, 4, 6));
  // printf("\n");
  // printf("%i\n", Board_wallBetween(board, 3, 5, 4, 5));
  // printf("%i\n", Board_wallBetween(board, 3, 5, 3, 6));
  // printf("%i\n", Board_wallBetween(board, 3, 5, 4, 6));
  // printf("\n");
  // printf("%i\n", GameState_legalMove(state, PLAYER1, 4, 5));
  // printf("%i\n", GameState_legalMove(state, PLAYER1, 3, 6));
  // printf("\n");

  return 0;
}
