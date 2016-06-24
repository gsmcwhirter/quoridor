#include "ai.h"
#include "gamestate.h"
#include "history.h"


// typedef struct PossibleMoves {
//   int wall_size;
//   gamemove_t **wall_moves;
//   int walk_size;
//   gamemove_t **walk_moves;
// } possmoves_t;

possmoves_t *
PossibleMoves_create()
{
  possmoves_t *pm = malloc(sizeof(possmoves_t));
  pm->wall_size = 0;
  pm->wall_moves = NULL;
  pm->walk_size = 0;
  pm->walk_moves = NULL;

  return pm;
}


possmoves_t *
PossibleMoves_clone(possmoves_t *pm)
{
  possmoves_t *new = PossibleMoves_create();
  new->wall_size = pm->wall_size;
  new->walk_size = pm->walk_size;

  if (new->wall_size > 0)
  {
    new->wall_moves = malloc(sizeof(gamemove_t *) * new->wall_size);
    for (int i = 0; i < new->wall_size; i++)
    {
      *(new->wall_moves + i) = GameMove_clone(*(pm->wall_moves + i));
    }
  }

  if (new->walk_size > 0)
  {
    new->walk_moves = malloc(sizeof(gamemove_t *) * new->walk_size);
    for (int i = 0; i < new->walk_size; i++)
    {
      *(new->walk_moves + i) = GameMove_clone(*(pm->walk_moves + i));
    }
  }

  return new;
}

void
PossibleMoves_destroy(possmoves_t *pm)
{
  if (pm != NULL)
  {
    if (pm->wall_moves != NULL)
    {
      for (int i = 0; i < pm->wall_size; i++)
      {
        GameMove_destroy(*(pm->wall_moves + i));
      }
      free(pm->wall_moves);
    }

    if (pm->walk_moves != NULL)
    {
      for (int i = 0; i < pm->walk_size; i++)
      {
        GameMove_destroy(*(pm->walk_moves + i));
      }
      free(pm->walk_moves);
    }

    free(pm);
  }
}


// typedef struct AIStage {
//   int my_player;
//   gamestate_t *gamestate;
//   gamehistory_t *history;
//   possmoves_t *nextmoves;
//   int stage_score;
//   struct AIStage *prev;
// } aistage_t;

aistage_t *
AIStage_create(gamestate_t *gamestate, gamehistory_t *history)
{
  aistage_t *ais = malloc(sizeof(aistage_t));

  ais->my_player = gamestate->player;
  ais->gamestate = GameState_clone(gamestate);
  ais->history = GameHistory_clone(history);
  ais->nextmoves = NULL;

  ais->stage_score = AIStage_evaluateGameState(ais);
  ais->prev = NULL;

  return ais;
}

aistage_t *
AIStage_advance(aistage_t *oldais, gamemove_t *move) //clone equiv
{
    aistage_t *ais = malloc(sizeof(aistage_t));

    gamemove_t *newmove = GameMove_clone(move);

    ais->gamestate = GameState_clone(oldais->gamestate);
    GameState_applyMove(ais->gamestate, newmove);
    ais->history = GameHistory_clone(oldais->history);
    GameHistory_push(ais->history, newmove);
    ais->nextmoves = PossibleMoves_clone(oldais->nextmoves);
    AIStage_updatePossibleMoves(ais);
    ais->stage_score = AIStage_evaluateGameState(ais);
    ais->prev = oldais;

    return ais;
}


void
AIStage_destroy(aistage_t *ais)
{
  if (ais != NULL)
  {
    GameState_destroy(ais->gamestate);
    GameHistory_destroy(ais->history);
    PossibleMoves_destroy(ais->nextmoves);
    free(ais);
  }
}


void AIStage_generatePossibleMoves(aistage_t *ais);
void AIStage_updatePossibleMoves(aistage_t *ais);
int AIStage_evaluateGameState(aistage_t *ais);
