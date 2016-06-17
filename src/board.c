#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "term/term.h"


board_t *
Board_create()
{
  board_t *board = malloc(sizeof(board_t));
  //board->size = size;

  //int bits = (size * 2 + 1) * (size * 2 + 1);
  //int ints = bits / sizeof(wall_field_t) + (bits % sizeof(wall_field_t) > 0 ? 1 : 0);

  //board->wall_size = ints;
  //board->walls = calloc(board->wall_size, sizeof(wall_field_t));
  board->walls = (bool *)calloc(WALLS_SIZE_SQ, sizeof(bool));
  for (int i = 0; i < WALLS_SIZE; i++)
  {
    int indexcol0 = WALLS_SIZE * i;
    int indexcol11 = indexcol0 + (WALLS_SIZE - 1);
    int indexrow0 = i;
    int indexrow11 = WALLS_SIZE_SQ - (WALLS_SIZE - i);
    board->walls[indexcol0] = true;
    board->walls[indexcol11] = true;
    board->walls[indexrow0] = true;
    board->walls[indexrow11] = true;
  }
  board->player1[0] = 1;
  board->player1[1] = 5;
  board->player2[0] = 9;
  board->player2[1] = 5;

  return board;
}


board_t *
Board_clone(const board_t *board)
{
  board_t *new_board = malloc(sizeof(board_t));
  //new_board->size = board->size;
  //new_board->wall_size = board->wall_size;
  //new_board->walls = calloc(board->wall_size, sizeof(wall_field_t));
  new_board->walls = malloc(WALLS_SIZE_SQ * sizeof(bool));
  for (int i = 0; i < WALLS_SIZE_SQ; i++)
  {
    *(new_board->walls + i) = *(board->walls + i);
  }

  new_board->player1[0] = board->player1[0];
  new_board->player1[1] = board->player1[1];
  new_board->player2[0] = board->player2[0];
  new_board->player2[1] = board->player2[1];

  return new_board;
}


void
Board_destroy(board_t *board)
{
  if (board != NULL)
  {
    free(board->walls);
    free(board);
  }
}


void
Board_print(board_t *board)
{
  //term_clear("screen");
  for (int r = WALLS_SIZE - 1; r >= 0; r--)
  {
    for (int c = 0; c < WALLS_SIZE; c++)
    {
      if (c == 0 && r % 2 == 0)
      {
        printf("  ");
      }
      else if (c == 0)
      {
        term_color(LABEL_COLOR);
        printf("%i ", (r + 1) / 2);
        term_reset();
      }

      int wi = r * WALLS_SIZE + c;
      if (c % 2 == 0 && r % 2 == 0) //wall intersections
      {
        if (*(board->walls + wi))
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
      else if (c % 2 == 0) //vertical walls
      {
        if (*(board->walls + wi))
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
      else if (r % 2 == 0) //horizontal walls
      {
        if (*(board->walls + wi))
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
      else //squares
      {
        if ((r+1)/2 == board->player1[0] && (c+1)/2 == board->player1[1])
        {
          term_color(PLAYER_1_COLOR);
          printf(" 1 ");
          term_reset();
        }
        else if ((r+1)/2 == board->player2[0] && (c+1)/2 == board->player2[1])
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
    printf("\n");
  }

  term_color(LABEL_COLOR);
  printf("  ");
  for (int c = 0; c < WALLS_SIZE; c++)
  {
    if (c % 2 == 0)
    {
      printf(" ");
    }
    else
    {
      char i = 'a' + (c - 1) / 2;
      printf(" %c ", i);
    }
  }
  term_reset();
  printf("\n");
}


void
Board_addWall(board_t *board, walldir_t walldir, int r, int c)
{
  int i1, i2, i3;
  //horizontal starts at the top-left, vertical at the bottom-right
  if (walldir == HORIZONTAL)
  {
    i1 = (c * 2 - 1) + (r * 2) * WALLS_SIZE;
    i2 = i1 + 1;
    i3 = i2 + 1;
  }
  else
  {
    i1 = (c * 2) + (r * 2 - 1) * WALLS_SIZE;
    i2 = i1 + WALLS_SIZE;
    i3 = i2 + WALLS_SIZE;
  }
  *(board->walls + i1) = true;
  *(board->walls + i2) = true;
  *(board->walls + i3) = true;
}


bool
Board_validWall(board_t *board, walldir_t walldir, int r, int c)
{
  if (c >= 9 || c <= 0) return false;
  if (r <= 0 || r >= 9) return false;

  int i1, i2, i3;
  //horizontal starts at the top-left, vertical at the bottom-right
  if (walldir == HORIZONTAL)
  {
    i1 = (c * 2 - 1) + (r * 2) * WALLS_SIZE;
    i2 = i1 + 1;
    i3 = i2 + 1;
  }
  else
  {
    i1 = (c * 2) + (r * 2 - 1) * WALLS_SIZE;
    i2 = i1 + WALLS_SIZE;
    i3 = i2 + WALLS_SIZE;
  }

  return !(*(board->walls + i1) || *(board->walls + i2) || *(board->walls + i3));
}


void
Board_movePlayer(board_t *board, player_t player, int r, int c)
{
  if (player == PLAYER1)
  {
    board->player1[0] = r;
    board->player1[1] = c;
  }
  else
  {
    board->player2[0] = r;
    board->player2[1] = c;
  }
}


bool
adjacentSquares(int r1, int c1, int r2, int c2)
{
  int sum = (r1-r2) + (c1-c2);
  return ((r1-r2) * (c1-c2) == 0) && (sum == 1 || sum == -1);
}


bool
Board_wallBetween(board_t *board, int r1, int c1, int r2, int c2)
{
  //assumes the squares are next to each other
  int r = ((r1 * 2) + (r2 * 2)) / 2 - 1;
  int c = ((c1 * 2) + (c2 * 2)) / 2 - 1;
  return *(board->walls + (r * WALLS_SIZE + c));
}
