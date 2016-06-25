#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "term/term.h"

#include "board.h"
#include "graph.h"


unsigned char
rcToLoc(unsigned char r, char c)
{
  return (r - 1) * SQUARES_SIZE + (c - 'a');
}

unsigned char
locToRow(unsigned char loc)
{
  return loc / SQUARES_SIZE + 1;
}

unsigned char
locToCol(unsigned char loc)
{
  return loc % SQUARES_SIZE;
}


// board_t *
// Board_create()
// {
//   board_t *board = malloc(sizeof(board_t));
//
//   Board_init(board);
//
//   return board;
// }

void
Board_init(board_t *board)
{
  board->wall_slots = 0;
  board->player1 = rcToLoc(1, 'e');
  board->player2 = rcToLoc(9, 'e');
  Graph_init(&(board->squares), SQUARES_SIZE_SQ, 4);

  unsigned char r;
  char c;
  // Create edges
  for (int i = 0; i < SQUARES_SIZE_SQ; i++)
  {
    r = locToRow(i);
    c = 'a' +locToCol(i);

    if (r + 1 <= SQUARES_SIZE)
      Graph_addEdge(&(board->squares), i, rcToLoc(r + 1, c));
    if (r > 1)
      Graph_addEdge(&(board->squares), i, rcToLoc(r - 1, c));
    if (c + 1 < 'a' + SQUARES_SIZE)
      Graph_addEdge(&(board->squares), i, rcToLoc(r, c + 1));
    if (c - 1 >= 'a')
      Graph_addEdge(&(board->squares), i, rcToLoc(r, c - 1));

  }
}


board_t *
Board_clone(const board_t *board, board_t *new_board)
{
  if (new_board == NULL)
  {
    new_board = malloc(sizeof(board_t));
  }

  new_board->wall_slots = board->wall_slots;

  new_board->player1 = board->player1;
  new_board->player2 = board->player2;

  Graph_clone(&(board->squares), &(new_board->squares));

  return new_board;
}


void
Board_destroy(board_t *board)
{
  // if (board != NULL)
  // {
  //   Graph_destroy(&(board->squares));
  //   free(board);
  // }
}


bool
Board_isWallAtCorner(board_t *board, unsigned char r, char col)
{
  unsigned char c = col - 'a' + 1;
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
Board_setWallAtCorner(board_t *board, unsigned char r, char col)
{
  int c = col - 'a' + 1;
  if (r > 0 && r <= WALLS_SIZE && c > 0 && c <= WALLS_SIZE)
  {
    int bit = (r-1) * WALLS_SIZE + (c-1);

    board->wall_slots = (board->wall_slots | (1ll<<bit));
  }
}

// void
// printWallSlots(unsigned long long ws)
// {
//   for (int i = 63; i >= 0; i--)
//   {
//     printf("%i", (ws & (1ll<<i))>0ll);
//   }
//   printf("\n");
// }

void
Board_printHwall(board_t *board, unsigned char r, char col)
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
Board_printCorner(board_t *board, unsigned char r, char col)
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
Board_printHWalls(board_t *board, player_t as_player, unsigned char r)
{
  for (char cc = 0; cc <= SQUARES_SIZE; cc++)
  {
    char c;
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
Board_printVWall(board_t *board, unsigned char r, char col)
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
Board_printSquare(board_t *board, unsigned char r, char col)
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
      if (r == locToRow(board->player1) && col == 'a' + locToCol(board->player1))
      {
        term_color(PLAYER_1_COLOR);
        printf(" 1 ");
        term_reset();
      }
      else if (r == locToRow(board->player2) && col == 'a' + locToCol(board->player2))
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
Board_printVWalls(board_t *board, player_t as_player, unsigned char r)
{
  for (char cc = 0; cc <= SQUARES_SIZE; cc++)
  {
    char c;
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
  for (char rr = SQUARES_SIZE; rr >= 0; rr--)
  {
    unsigned char r;
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
Board_wallBetween(board_t *board, unsigned char r1, char c1, unsigned char r2, char c2)
{
  if (r1 == 0 || r2 == 0 || c1 < 'a' || c2 < 'a') return true;
  //assumes the squares are next to each other
  unsigned char loc1 = rcToLoc(r1, c1);
  unsigned char loc2 = rcToLoc(r2, c2);
  if (Graph_hasEdge(&(board->squares), loc1, loc2))
  {
    return false;
  }
  else
  {
    return true;
  }
}


bool
Board_addWall(board_t *board, walldir_t walldir, unsigned char r, char c)
{
  unsigned char n = rcToLoc(r, c);

  Board_setWallAtCorner(board, r, c);
  //horizontal starts at the top-left, vertical at the bottom-right
  if (walldir == HORIZONTAL)
  {
    // cut edge between me and the one above, and same to the right
    Graph_removeEdge(&(board->squares), n, n+SQUARES_SIZE);
    Graph_removeEdge(&(board->squares), n+1, n+1+SQUARES_SIZE);
  }
  else if (walldir == VERTICAL) //vertical
  {
    // cut edge between me and the one to the right, and the same above
    Graph_removeEdge(&(board->squares), n, n+1);
    Graph_removeEdge(&(board->squares), n+SQUARES_SIZE, n+SQUARES_SIZE+1);
  }
  else
  {
    return false;
  }

  return true;
}


bool
Board_movePlayer(board_t *board, player_t player, unsigned char r, char c)
{
  if (player == PLAYER1)
  {
    board->player1 = rcToLoc(r, c);
  }
  else
  {
    board->player2 = rcToLoc(r, c);;
  }
  return true;
}


bool
Board_validWall(board_t *board, walldir_t walldir, unsigned char r, char c)
{
  // printf("%i %c\n", r, c);
  if (Board_isWallAtCorner(board, r, c)) return false;

  int n = rcToLoc(r, c);
  // printf("%i\n", n);

  if (walldir == HORIZONTAL && Graph_hasEdge(&(board->squares), n, n+SQUARES_SIZE) && Graph_hasEdge(&(board->squares), n+1, n+1+SQUARES_SIZE))
    return true;
  else if (walldir == VERTICAL && Graph_hasEdge(&(board->squares), n, n+1) && Graph_hasEdge(&(board->squares), n+SQUARES_SIZE, n+SQUARES_SIZE+1))
    return true;
  else
    return false;
}
