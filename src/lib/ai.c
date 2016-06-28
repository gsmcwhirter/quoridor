#include <stdio.h>
#include <stdbool.h>

#include "ai.h"
#include "board.h"
#include "gamestate.h"
#include "history.h"
#include "search.h"
#include "move.h"

#define WALL_SCORE 3
#define PATH_SCORE 1
#define BLOCKED_SCORE -10000000
#define BEST_TOLERANCE 1


// typedef struct PossibleMoves {
//   int wall_size;
//   gamemove_t **wall_moves;
//   int walk_size;
//   gamemove_t **walk_moves;
// } possmoves_t;

void
PossibleMoves_init(possmoves_t *pm)
{
  pm->wall_size = 0;
  pm->walk_size = 0;
}


possmoves_t *
PossibleMoves_clone(const possmoves_t *pm, possmoves_t *new)
{
  if (new == NULL)
  {
    new = malloc(sizeof(possmoves_t));
  }
  new->wall_size = pm->wall_size;
  new->walk_size = pm->walk_size;

  for (int i = 0; i < new->wall_size; i++)
  {
    GameMove_clone(&(pm->wall_moves[i]), &(new->wall_moves[i]));
  }

  for (int i = 0; i < new->walk_size; i++)
  {
    GameMove_clone(&(pm->walk_moves[i]), &(new->walk_moves[i]));
  }

  return new;
}

// void
// PossibleMoves_destroy(possmoves_t *pm)
// {
//   if (pm != NULL)
//   {
//     if (pm->wall_moves != NULL)
//     {
//       for (int i = 0; i < pm->wall_size; i++)
//       {
//         GameMove_destroy(*(pm->wall_moves + i));
//       }
//       free(pm->wall_moves);
//     }
//
//     if (pm->walk_moves != NULL)
//     {
//       for (int i = 0; i < pm->walk_size; i++)
//       {
//         GameMove_destroy(*(pm->walk_moves + i));
//       }
//       free(pm->walk_moves);
//     }
//
//     free(pm);
//   }
// }


// typedef struct AIStage {
//   int my_player;
//   gamestate_t *gamestate;
//   gamehistory_t *history;
//   possmoves_t *nextmoves;
//   int stage_score;
//   struct AIStage *prev;
// } aistage_t;

void
AIStage_init(aistage_t *ais, gamestate_t *gamestate, gamehistory_t *history)
{
  ais->my_player = gamestate->player;
  GameState_clone(gamestate, &(ais->gamestate));
  GameHistory_clone(history, &(ais->history));
  PossibleMoves_init(&(ais->nextmoves));
  ais->stage_score_defined = false;
  // ais->prev = NULL;

  // return ais;
}

aistage_t *
AIStage_advance(const aistage_t *oldais, aistage_t *ais, gamemove_t *move) //clone equiv
{
  if (ais == NULL)
  {
    ais = malloc(sizeof(aistage_t));
  }

  //gamemove_t *newmove = GameMove_clone(move);

  GameState_clone(&(oldais->gamestate), &(ais->gamestate));
  GameState_applyMove(&(ais->gamestate), move);
  GameHistory_clone(&(oldais->history), &(ais->history));
  GameHistory_push(&(ais->history), move, &(ais->gamestate));
  PossibleMoves_clone(&(oldais->nextmoves), &(ais->nextmoves));
  AIStage_updatePossibleMoves(ais);
  ais->stage_score_defined = false;
  // ais->prev = oldais;

  return ais;
}


// void
// AIStage_destroy(aistage_t *ais)
// {
//   if (ais != NULL)
//   {
//     ais->prev = NULL;
//     GameState_destroy(ais->gamestate);
//     GameHistory_destroy(ais->history);
//     PossibleMoves_destroy(ais->nextmoves);
//     free(ais);
//   }
// }


void
AIStage_generateWalkMoves(aistage_t *ais)
{
  // if (ais->nextmoves.walk_size > 0)
  // {
  //   for (int i = 0; i < ais->nextmoves.walk_size; i++)
  //   {
  //     GameMove_destroy(*(ais->nextmoves.walk_moves + i));
  //   }
  // }

  int ct = 0;
  int r;
  char c;
  char * buffer = "    ";

  int loc, loc2;
  if (ais->gamestate.player == PLAYER1)
  {
    loc = ais->gamestate.board.player1;
    loc2 = ais->gamestate.board.player2;
  }
  else
  {
    loc = ais->gamestate.board.player2;
    loc2 = ais->gamestate.board.player1;
  }

  ct = 0;
  graph_t *g = &(ais->gamestate.board.squares);
  adjlist_t *al = Graph_neighbors(g, loc);
  adjlist_t *al2;
  int deg = al->degree;
  int neigh;
  for (int i = 0; i < deg; i++)
  {
    if (ais->gamestate.player == PLAYER1)
      neigh = *(al->neighbors + i);
    else
      neigh = *(al->neighbors + (deg - i - 1));

    if (neigh == loc2)
    {
      al2 = Graph_neighbors(g, loc2);
      if (AdjList_hasNeighbor(al2, loc2 + (loc2 - loc)))
      {
        r = locToRow(loc2 + (loc2 - loc));
        c = 'a' + locToCol(loc2 + (loc2 - loc));
        sprintf(buffer, "%c%i", c, r);
        GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r, c, NONE, buffer);
        // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c, NONE, buffer);
        ct++;
      }
      else
      {
        r = locToRow(loc2);
        c = 'a' + locToCol(loc2);

        if (abs(loc2-loc) == SQUARES_SIZE)
        {
          if (c - 1 >= 'a')
          {
            sprintf(buffer, "%c%i", c-1, r);
            GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r, c-1, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c-1, NONE, buffer);
            ct++;
          }

          if (c + 1 <= 'i')
          {
            sprintf(buffer, "%c%i", c+1, r);
            GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r, c+1, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c+1, NONE, buffer);
            ct++;
          }
        }
        else
        {
          if (r - 1 > 0)
          {
            sprintf(buffer, "%c%i", c, r-1);
            GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r-1, c, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r-1, c, NONE, buffer);
            ct++;
          }

          if (r + 1 <= SQUARES_SIZE)
          {
            sprintf(buffer, "%c%i", c, r+1);
            GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r+1, c, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r+1, c, NONE, buffer);
            ct++;
          }
        }
      }
    }
    else
    {
      r = locToRow(neigh);
      c = 'a' + locToCol(neigh);
      sprintf(buffer, "%c%i", c, r);
      GameMove_init(&(ais->nextmoves.walk_moves[ct]), ais->gamestate.player, r, c, NONE, buffer);
      // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c, NONE, buffer);
      ct++;
    }
  }

  ais->nextmoves.walk_size = ct;
}


void
AIStage_generatePossibleMoves(aistage_t *ais)
{
  // if (ais->nextmoves->wall_size > 0)
  // {
  //   for (int i = 0; i < ais->nextmoves->wall_size; i++)
  //   {
  //     GameMove_destroy(*(ais->nextmoves->wall_moves + i));
  //   }
  // }

  int ct = 0;
  int r;
  char c;
  char * buffer = "    ";
  for (int i = 0; i < WALLS_SIZE_SQ; i++)
  {
    r = locToRow(i);
    c = 'a' + locToCol(i);

    if (Board_validWall(&(ais->gamestate.board), HORIZONTAL, r, c))
    {
      sprintf(buffer, "%c%ih", c, r);
      GameMove_init(&(ais->nextmoves.wall_moves[ct]), ais->gamestate.player, r, c, HORIZONTAL, buffer);
      // *(ais->nextmoves->wall_moves + ct) = GameMove_create(ais->gamestate->player, r, c, HORIZONTAL, buffer);
      ct++;
    }

    if (Board_validWall(&(ais->gamestate.board), VERTICAL, r, c))
    {
      sprintf(buffer, "%c%iv", c, r);
      GameMove_init(&(ais->nextmoves.wall_moves[ct]), ais->gamestate.player, r, c, VERTICAL, buffer);
      // *(ais->nextmoves->wall_moves + ct) = GameMove_create(ais->gamestate->player, r, c, VERTICAL, buffer);
      ct++;
    }
  }

  ais->nextmoves.wall_size = ct;

  AIStage_generateWalkMoves(ais);
}


void AIStage_updatePossibleMoves(aistage_t *ais)
{
  AIStage_generateWalkMoves(ais);

  gamemove_t *move;
  for (int i = ais->nextmoves.wall_size - 1; i >= 0; i--)
  {
    move = &(ais->nextmoves.wall_moves[i]);
    if (!Board_validWall(&(ais->gamestate.board), move->wall, move->row, move->col))
    {
      ais->nextmoves.wall_size--;
      GameMove_clone(&(ais->nextmoves.wall_moves[ais->nextmoves.wall_size]), &(ais->nextmoves.wall_moves[i]));
      // GameMove_clone(&(ais->nextmoves.wall_moves[ais->nextmoves.wall_size]), &(ais->nextmoves.wall_moves[i]));
      // *(ais->nextmoves->wall_moves + i) = *(ais->nextmoves->wall_moves + ais->nextmoves->wall_size);
      // *(ais->nextmoves->wall_moves + ais->nextmoves->wall_size) = NULL;
      // GameMove_destroy(move);
    }
  }
}

int
AIStage_evaluateGameState(aistage_t *ais)
{
  if (ais->stage_score_defined)
    return ais->stage_score;

  int score = 0;
  int p1walls = WALL_SCORE * ais->gamestate.player1_walls;
  int p2walls = WALL_SCORE * ais->gamestate.player2_walls;

  int p1pathlen, p2pathlen;
  searchresult_t sr;
  SearchResult_init(&sr, 1, 0);
  // path_t p1path;
  // path_t p2path;

  Search_bfs_all(&sr, &(ais->gamestate.board), PLAYER1, ais->gamestate.board.player1);
  if (sr.count == 0)
  {
    ais->stage_score_defined = true;
    ais->stage_score = BLOCKED_SCORE;
    return BLOCKED_SCORE;
  }
  else
  {
    // p1path = *(sr->shortest_paths);
    p1pathlen = sr.shortest_paths[0].length;
  }

  SearchResult_reset(&sr);
  Search_bfs_all(&sr, &(ais->gamestate.board), PLAYER2, ais->gamestate.board.player2);
  if (sr.count == 0)
  {
    ais->stage_score_defined = true;
    ais->stage_score = BLOCKED_SCORE;
    return BLOCKED_SCORE;
  }
  else
  {
    // p2path = *(sr->shortest_paths);
    p2pathlen = p1pathlen = sr.shortest_paths[0].length;;
  }

  if (ais->gamestate.player == PLAYER1)
  {
    score = p1walls - p2walls + p2pathlen - p1pathlen;
  }
  else
  {
    score = p2walls - p1walls + p1pathlen - p2pathlen;
  }

  ais->stage_score_defined = true;
  ais->stage_score = score;
  return score;
}

moveseq_t *
MoveSequence_create()
{
  moveseq_t *bm = malloc(sizeof(moveseq_t));
  bm->num_moves = 0;
  bm->max_moves = 8;
  bm->score = BLOCKED_SCORE;
  bm->moves = malloc(sizeof(gamemove_t *) * bm->max_moves);

  return bm;
}

moveseq_t *
MoveSequence_clone(moveseq_t *old)
{
  moveseq_t *ms = malloc(sizeof(moveseq_t));
  ms->num_moves = old->num_moves;
  ms->max_moves = old->max_moves;
  ms->score = old->score;
  ms->moves = malloc(sizeof(gamemove_t *) * old->max_moves);

  for (int i = 0; i < old->num_moves; i++)
  {
    *(ms->moves + i) = *(old->moves + i);
  }

  return ms;
}

void
MoveSequence_add(moveseq_t *ms, gamemove_t *move)
{
  if (ms->num_moves == ms->max_moves)
  {
    MoveSequence_expand(ms);
  }

  *(ms->moves + ms->num_moves) = move;
  ms->num_moves++;
}

void
MoveSequence_expand(moveseq_t *ms)
{
  gamemove_t **old = ms->moves;
  ms->moves = malloc(sizeof(gamemove_t *) * ms->max_moves * 2);
  for (int i = 0; i < ms->num_moves; i++)
  {
    *(ms->moves + i) = *(old + i);
    *(old + i) = NULL;
  }
  free(old);
}

void
MoveSequence_destroy(moveseq_t *bm)
{
  if (bm != NULL)
  {
    for (int i = 0; i < bm->num_moves; i++)
    {
      *(bm->moves + i) = NULL;
    }
    free(bm->moves);
    free(bm);
  }
}

bestmoves_t *
BestMoves_create()
{
  bestmoves_t *bm = malloc(sizeof(bestmoves_t));
  bm->size = 0;
  bm->max_size = 8;
  bm->score = BLOCKED_SCORE + 1;
  bm->moves = malloc(sizeof(moveseq_t *) * (bm->max_size));

  return bm;
}

void
BestMoves_add(bestmoves_t *bm, moveseq_t *ms)
{
  if (bm->size == bm->max_size)
  {
    BestMoves_expand(bm);
  }

  *(bm->moves + bm->size) = ms;
  bm->size++;
}

void
BestMoves_expand(bestmoves_t *bm)
{
  moveseq_t **old = bm->moves;
  bm->moves = malloc(sizeof(moveseq_t *) * bm->max_size * 2);
  for (int i = 0; i < bm->size; i++)
  {
    *(bm->moves + i) = *(old + i);
    *(old + i) = NULL;
  }
  free(old);
}


void
BestMoves_destroy(bestmoves_t *bm)
{
  if (bm != NULL)
  {
    for (int i = 0; i < bm->size; i++)
    {
      MoveSequence_destroy(*(bm->moves + i));
      *(bm->moves + i) = NULL;
    }
    free(bm->moves);
    free(bm);
  }
}

bestmoves_t *
AIStage_bestMoves(aistage_t *ais, moveseq_t *ms, int lookahead)
{
  // odd lookahead is my turn, even is opponent's, but ignoring for now
  bestmoves_t *best = BestMoves_create();
  bestmoves_t *results;
  moveseq_t *newms;
  aistage_t next;

  if (ms == NULL)
  {
    ms = MoveSequence_create();
  }



  //TODO: duplicate everything for walk instead of wall

  for (int i = 0; i < ais->nextmoves.wall_size; i++)
  {
    AIStage_advance(ais, &next, &(ais->nextmoves.wall_moves[i]));

    if (lookahead == 1)
    {
      AIStage_evaluateGameState(&next);
      if (next.stage_score > best->score)
      {
        best->score = next.stage_score; // now defined
        for (int k = best->size - 1; k >= 0 ; k--)
        {
          if ((*(best->moves + k))->score < best->score - BEST_TOLERANCE)
          {
            MoveSequence_destroy(*(best->moves + k));
            best->size--;
            *(best->moves + k) = *(best->moves + best->size);
            *(best->moves + best->size) = NULL;
          }
        }
      }

      if (next.stage_score >= best->score - BEST_TOLERANCE)
      {
        newms = MoveSequence_clone(ms);
        MoveSequence_add(newms, &(ais->nextmoves.wall_moves[i]));
        newms->score = next.stage_score;
        BestMoves_add(best, newms);
      }
    }
    else if (lookahead % 2 == 0)
    {
      // like the else, but minimize instead of maximize
      // TODO
    }
    else
    {
      newms = MoveSequence_clone(ms);
      MoveSequence_add(newms, &(ais->nextmoves.wall_moves[i]));
      results = AIStage_bestMoves(&next, newms, lookahead - 1);

      if (best->score < results->score - BEST_TOLERANCE) // results are far better
      {
        BestMoves_destroy(best);
        best = results;
      }
      else if (results->score < best->score - BEST_TOLERANCE) //best is far better
      {
        BestMoves_destroy(results);
      }
      else //mixed results
      {
        for (int b = 0; b < results->size; b++)
        {
          if ((*(results->moves + b))->score > best->score)
          {
            best->score = (*(results->moves + b))->score;
            for (int k = best->size - 1; k >= 0 ; k--)
            {
              if ((*(best->moves + k))->score < best->score - BEST_TOLERANCE)
              {
                MoveSequence_destroy(*(best->moves + k));
                best->size--;
                *(best->moves + k) = *(best->moves + best->size);
                *(best->moves + best->size) = NULL;
              }
            }
          }

          if ((*(results->moves + b))->score >= best->score - BEST_TOLERANCE)
          {
            BestMoves_add(best, *(results->moves + b));
          }
        }
      }
    }
  }

  MoveSequence_destroy(ms);

  return best;

}
