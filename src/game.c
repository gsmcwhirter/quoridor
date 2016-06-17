#include <stdio.h>
#include <stdbool.h>

#include "commander/commander.h"
#include "strdup/strdup.h"
#include "gamestate.h"

static bool verbose = false;
static char* history = "";


static void
setVerbose(command_t *self)
{
  printf("verbose: enabled\n");
  verbose = true;
}

static void
loadHistory(command_t *self)
{
  printf("loading history: %s\n", self->arg);
  history = strdup(self->arg);
}

int
main(int argc, const char **argv)
{
  command_t cmd;
  command_init(&cmd, argv[0], "0.0.1");
  command_option(&cmd, "-v", "--verbose", "enable verbose stuff", setVerbose);
  command_option(&cmd, "-h", "--history [history_str]", "history to play-back", loadHistory);
  command_free(&cmd);
  return 0;
}

void
repl()
{

}
