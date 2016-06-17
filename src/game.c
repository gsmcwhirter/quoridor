#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "commander/commander.h"
#include "strdup/strdup.h"
#include "occurrences/occurrences.h"
#include "case/case.h"
#include "trim/trim.h"
#include "strsplit/strsplit.h"
#include "board.h"
#include "gamestate.h"

#define UNUSED(x) (void)(x)

bool verbose = false;
char* history = "";
char* history_delim = ",";

char buffer[40];

static void
setVerbose(command_t *self)
{
  UNUSED(self);
  printf("verbose: enabled\n");
  verbose = true;
}

static void
loadHistory(command_t *self)
{
  printf("loading history: %s\n", self->arg);
  history = trim(case_lower(strdup(self->arg)));
}

static void
historyDelimeter(command_t *self)
{
  printf("setting history delimeter: %s\n", self->arg);
  history_delim = strdup(self->arg);
}

//The following function is from http://home.datacomm.ch/t_wolf/tw/c/getting_input.html
char *
read_line (char *buf, size_t length, FILE *f)
  /**** Read at most 'length'-1 characters from the file 'f' into
        'buf' and zero-terminate this character sequence. If the
        line contains more characters, discard the rest.
   */
{
  char *p;

  if (p = fgets (buf, length, f)) {
    size_t last = strlen (buf) - 1;

    if (buf[last] == '\n') {
      /**** Discard the trailing newline */
      buf[last] = '\0';
    } else {
      /**** There's no newline in the buffer, therefore there must be
            more characters on that line: discard them!
       */
      fscanf (f, "%*[^\n]");
      /**** And also discard the newline... */
      (void) fgetc (f);
    } /* end if */
  } /* end if */
  return p;
} /* end read_line */

void
repl()
{
  printf("\n");
  printf("Starting Game...\n");
  size_t ct = occurrences(history_delim, history);

  size_t num_moves;
  char **moves;

  if (ct > 0)
  {
    moves = malloc(sizeof(char*) * ct);
    num_moves = strsplit(history, moves, history_delim);
  }
  else
  {
    moves = malloc(sizeof(char*));
    *moves = strdup(history);
    num_moves = strlen(trim(*(moves))) > 0 ? 1 : 0;
  }

  player_t player;

  if (num_moves > 0)
  {
    if (num_moves % 2 == 0)
    {
      player = PLAYER1;
    }
    else
    {
      player = PLAYER2;
    }
  }
  else
  {
    player = PLAYER1;
  }

  for (unsigned int i = 0; i < num_moves; i++)
  {
    printf("Replaying move: %s\n", *(moves + i));
    // TODO: replay move
  }

  board_t *board = Board_create();
  gamestate_t *gamestate = GameState_create(board,player);
  GameState_print(gamestate);

  while (true)
  {
    char *move;
    printf("Enter next move (empty to autocompute): ");
    move = read_line(buffer, 40, stdin);
    printf("%s", move);
    printf("\n");
    break;
  }
}

int
main(int argc, char **argv)
{
  command_t cmd;
  command_init(&cmd, argv[0], "0.0.1");
  command_option(&cmd, "-v", "--verbose", "enable verbose stuff", setVerbose);
  command_option(&cmd, "-H", "--history [arg]", "history to play-back (default \"\")", loadHistory);
  command_option(&cmd, "-d", "--delimeter [arg]", "history delimeter (default \",\")", historyDelimeter);
  command_parse(&cmd, argc, argv);

  repl();

  command_free(&cmd);
  return 0;
}
