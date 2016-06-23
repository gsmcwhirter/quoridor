#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "graph.h"
#include "term/term.h"


int
locToInt(int r, char c, int row_size)
{
  return (r - 1) * row_size + ((int)(c - 'a'));
}

int
intToRow(int loc, int row_size)
{
  return loc / row_size + 1;
}

int
intToCol(int loc, int row_size)
{
  return loc % row_size;
}


board_t *
Board_create()
{
  board_t *board = malloc(sizeof(board_t));

  board->wall_slots = 0;
  board->player1 = locToInt(1, 'e', SQUARES_SIZE);
  board->player2 = locToInt(9, 'e', SQUARES_SIZE);
  board->squares = Graph_create(SQUARES_SIZE_SQ, 4);

  int r;
  char c;
  // Create edges
  for (int i = 0; i < SQUARES_SIZE_SQ; i++)
  {
    r = intToRow(i, SQUARES_SIZE);
    c = 'a' + intToCol(i, SQUARES_SIZE);

    int ns[4] = {-1, -1, -1, -1};
    if (r + 1 <= SQUARES_SIZE)
      ns[0] = locToInt(r + 1, c, SQUARES_SIZE);
    if (r - 1 > 0)
      ns[1] = locToInt(r - 1, c, SQUARES_SIZE);
    if (c + 1 < 'a' + SQUARES_SIZE)
      ns[2] = locToInt(r, c + 1, SQUARES_SIZE);
    if (c - 1 >= 'a')
      ns[3] = locToInt(r, c - 1, SQUARES_SIZE);

    for (int j = 0; j < 4; j++)
    {
      if (ns[j] >= 0)
        Graph_addEdge(board->squares, i, ns[j]);
    }
  }

  return board;
}


board_t *
Board_clone(const board_t *board)
{
  board_t *new_board = malloc(sizeof(board_t));
  new_board->wall_slots = board->wall_slots;

  new_board->player1 = board->player1;
  new_board->player2 = board->player2;

  new_board->squares = Graph_clone(board->squares);

  return new_board;
}


void
Board_destroy(board_t *board)
{
  if (board != NULL)
  {
    Graph_destroy(board->squares);
    free(board);
  }
}


bool
Board_isWallAtCorner(board_t *board, int r, char col)
{
  int c = col - 'a' + 1;
  if (r == 0 || r > WALLS_SIZE) return true;
  if (c == 0 || c > WALLS_SIZE) return true;

  int bit = (r-1) * WALLS_SIZE + (c-1);
  if ((board->wall_slots & (1ll<<bit)) > 0ll)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void
Board_setWallAtCorner(board_t *board, int r, char col)
{
  int c = col - 'a' + 1;
  if (r > 0 && r <= WALLS_SIZE && c > 0 && c <= WALLS_SIZE)
  {
    int bit = (r-1) * WALLS_SIZE + (c-1);

    board->wall_slots = (board->wall_slots | (1ll<<bit));
  }
}

void
printWallSlots(unsigned long long ws)
{
  for (int i = 63; i >= 0; i--)
  {
    printf("%i", (ws & (1ll<<i))>0ll);
  }
  printf("\n");
}

void
Board_printHwall(board_t *board, int r, char col)
{
  if (col < 'a')
    printf("   "); //label blank on horizontal
  else
  {
    //print horizontal wall
    if (Board_wallBetween(board, r, col, r+1, col))
    {
      term_bold();
      printf("---");
      term_reset();
    }
    else
    {
      printf("   ");
    }
  }
}

void
Board_printCorner(board_t *board, int r, char col)
{
  //print corner
  if (Board_isWallAtCorner(board, r, col))
  {
    term_bold();
    printf("+");
    term_reset();
  }
  else
  {
    term_color("grey");
    printf("·");
    term_reset();
  }
}

void
Board_printHWalls(board_t *board, player_t as_player, int r)
{
  for (int cc = 0; cc <= SQUARES_SIZE; cc++)
  {
    int c;
    if (as_player == PLAYER2)
      c = SQUARES_SIZE - cc;
    else
      c = cc;

    char col = 'a' + c - 1;

    if (as_player == PLAYER2)
    {
      Board_printCorner(board, r, col);
      Board_printHwall(board, r, col);
    }
    else
    {
      Board_printHwall(board, r, col);
      Board_printCorner(board, r, col);
    }
  }
}



void
Board_printVWall(board_t *board, int r, char col)
{
  if (r > 0)
  {
    if (col < 'a')
    {
      term_bold();
      printf("|");
      term_reset();
    }
    else
    {
      if (Board_wallBetween(board, r, col, r, col+1))
      {
        term_bold();
        printf("|");
        term_reset();
      }
      else
      {
        printf(" ");
      }
    }
  }
  else
  {
    printf(" ");
  }
}


void
Board_printSquare(board_t *board, int r, char col)
{
  if (r > 0)
  {
    if (col < 'a')
    {
      term_color(LABEL_COLOR);
      printf(" %i ", r);
      term_reset();
    }
    else
    {
      // square
      if (r == intToRow(board->player1, SQUARES_SIZE) && col == 'a' + intToCol(board->player1, SQUARES_SIZE))
      {
        term_color(PLAYER_1_COLOR);
        printf(" 1 ");
        term_reset();
      }
      else if (r == intToRow(board->player2, SQUARES_SIZE) && col == 'a' + intToCol(board->player2, SQUARES_SIZE))
      {
        term_color(PLAYER_2_COLOR);
        printf(" 2 ");
        term_reset();
      }
      else
      {
        printf("   ");
      }
    }
  }
  else
  {
    if (col < 'a')
    {
      printf("   ");
    }
    else
    {
      term_color(LABEL_COLOR);
      printf(" %c ", col);
      term_reset();
    }
  }
}


void
Board_printVWalls(board_t *board, player_t as_player, int r)
{
  for (int cc = 0; cc <= SQUARES_SIZE; cc++)
  {
    int c;
    if (as_player == PLAYER2)
      c = SQUARES_SIZE - cc;
    else
      c = cc;

    char col = 'a' + c - 1;
    if (as_player == PLAYER2)
    {
      Board_printVWall(board, r, col);
      Board_printSquare(board, r, col);
    }
    else
    {
      Board_printSquare(board, r, col);
      Board_printVWall(board, r, col);
    }
  }
}


void
Board_print(board_t *board, player_t as_player)
{
  for (int rr = SQUARES_SIZE; rr >= 0; rr--)
  {
    int r;
    if (as_player == PLAYER2)
    {
      r = SQUARES_SIZE - rr;
      Board_printVWalls(board, as_player, r);
      printf("\n");
      Board_printHWalls(board, as_player, r);
      printf("\n");
    }
    else
    {
      r = rr;
      Board_printHWalls(board, as_player, r);
      printf("\n");
      Board_printVWalls(board, as_player, r);
      printf("\n");
    }
  }
}

bool
Board_wallBetween(board_t *board, int r1, char c1, int r2, char c2)
{
  if (r1 == 0 || r2 == 0 || c1 < 'a' || c2 < 'a') return true;
  //assumes the squares are next to each other
  int loc1 = locToInt(r1, c1, SQUARES_SIZE);
  int loc2 = locToInt(r2, c2, SQUARES_SIZE);
  if (Graph_hasEdge(board->squares, loc1, loc2))
  {
    return false;
  }
  else
  {
    return true;
  }
}


bool
Board_addWall(board_t *board, walldir_t walldir, int r, char c)
{
  int n = locToInt(r, c, SQUARES_SIZE);

  Board_setWallAtCorner(board, r, c);
  //horizontal starts at the top-left, vertical at the bottom-right
  if (walldir == HORIZONTAL)
  {
    // cut edge between me and the one above, and same to the right
    Graph_removeEdge(board->squares, n, n+SQUARES_SIZE);
    Graph_removeEdge(board->squares, n+1, n+1+SQUARES_SIZE);
  }
  else if (walldir == VERTICAL) //vertical
  {
    // cut edge between me and the one to the right, and the same above
    Graph_removeEdge(board->squares, n, n+1);
    Graph_removeEdge(board->squares, n+SQUARES_SIZE, n+SQUARES_SIZE+1);
  }
  else
  {
    return false;
  }

  return true;
}


bool
Board_movePlayer(board_t *board, player_t player, int r, char c)
{
  if (player == PLAYER1)
  {
    board->player1 = locToInt(r, c, SQUARES_SIZE);
  }
  else
  {
    board->player2 = locToInt(r, c, SQUARES_SIZE);;
  }
  return true;
}


bool
Board_validWall(board_t *board, walldir_t walldir, int r, char c)
{
  // printf("%i %c\n", r, c);
  if (Board_isWallAtCorner(board, r, c)) return false;

  int n = locToInt(r, c, SQUARES_SIZE);
  // printf("%i\n", n);

  if (walldir == HORIZONTAL && Graph_hasEdge(board->squares, n, n+SQUARES_SIZE) && Graph_hasEdge(board->squares, n+1, n+1+SQUARES_SIZE))
    return true;
  else if (walldir == VERTICAL && Graph_hasEdge(board->squares, n, n+1) && Graph_hasEdge(board->squares, n+SQUARES_SIZE, n+SQUARES_SIZE+1))
    return true;
  else
    return false;
}
