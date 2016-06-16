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
  term_clear("screen");
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
        term_color("cyan");
        printf("%i ", (r + 1) / 2);
        term_reset();
      }

      int wi = r * WALLS_SIZE + c;
      if (c % 2 == 0 && r % 2 == 0) //wall intersections
      {
        if (*(board->walls + wi))
        {
          printf("+");
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
          printf("|");
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
          printf("---");
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
          term_color("red");
          printf(" 1 ");
          term_reset();
        }
        else if ((r+1)/2 == board->player2[0] && (c+1)/2 == board->player2[1])
        {
          term_color("yellow");
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

  term_color("cyan");
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
