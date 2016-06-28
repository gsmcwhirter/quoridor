#include <stdio.h>
#include <stdbool.h>

#include "strdup/strdup.h"

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

  for (int i = 5; i < 5 + new->wall_size; i++)
  {
    GameMove_clone(&(pm->moves[i]), &(new->moves[i]));
  }

  for (int i = 0; i < new->walk_size; i++)
  {
    GameMove_clone(&(pm->moves[i]), &(new->moves[i]));
  }

  return new;
}

void
PossibleMoves_print(possmoves_t *pm)
{
  printf("Walk Moves: %i; Wall Moves: %i\n", pm->walk_size, pm->wall_size);
  for (int i = 0; i < 5 + pm->wall_size; i++)
  {
    if (i < 5 && i >= pm->walk_size) continue;
    printf("%i - ", i);
    GameMove_print(&(pm->moves[i]));
    printf("\n");
  }
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
  #ifdef DEBUGAI
    printf("AIS Setting player...\n");
  #endif
  ais->my_player = gamestate->player;
  #ifdef DEBUGAI
    printf("AIS Cloning state...\n");
  #endif
  GameState_clone(gamestate, &(ais->gamestate));
  #ifdef DEBUGAI
    printf("AIS done state.\n");
  #endif
  #ifdef DEBUGAI
    printf("AIS Cloning history...\n");
  #endif
  GameHistory_print(history);
  GameHistory_clone(history, &(ais->history));
  #ifdef DEBUGAI
    printf("AIS done history.\n");
  #endif
  #ifdef DEBUGAI
    printf("AIS Initializing possible moves...\n");
  #endif
  PossibleMoves_init(&(ais->nextmoves));
  #ifdef DEBUGAI
    printf("AIS done possible moves.\n");
  #endif
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
  #ifdef DEBUGAI
    printf("Cloning state.\n");
  #endif
  GameState_clone(&(oldais->gamestate), &(ais->gamestate));
  #ifdef DEBUG
    printf("Advancing state.\n");
  #endif
  GameState_applyMove(&(ais->gamestate), move);
  #ifdef DEBUGAI
    printf("Cloning history.\n");
  #endif
  GameHistory_clone(&(oldais->history), &(ais->history));
  #ifdef DEBUGAI
    printf("Advancing history.\n");
  #endif
  GameHistory_push(&(ais->history), move, &(ais->gamestate));
  #ifdef DEBUGAI
    printf("Cloning and updating possible moves.\n");
  #endif
  PossibleMoves_clone(&(oldais->nextmoves), &(ais->nextmoves));
  AIStage_updatePossibleMoves(ais);
  ais->stage_score_defined = false;
  // ais->prev = oldais;

  return ais;
}


void
AIStage_generateWalkMoves(aistage_t *ais)
{
  #ifdef DEBUG
    printf("Generating walking moves...\n");
  #endif
  int ct = 0;
  int r;
  char c;
  char buffer[] = "    ";

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
  #ifdef DEBUG
    printf("going into loop...\n");
  #endif
  for (int i = 0; i < deg; i++)
  {
    if (ais->gamestate.player == PLAYER1)
      neigh = *(al->neighbors + i);
    else
      neigh = *(al->neighbors + (deg - i - 1));

    #ifdef DEBUG
      printf("considering neighbor %i...\n", neigh);
    #endif

    if (neigh == loc2)
    {
      #ifdef DEBUG
        printf("Other player is in that spot...\n");
      #endif
      al2 = Graph_neighbors(g, loc2);
      if (AdjList_hasNeighbor(al2, loc2 + (loc2 - loc)))
      {
        r = locToRow(loc2 + (loc2 - loc));
        c = 'a' + locToCol(loc2 + (loc2 - loc));
        sprintf(buffer, "%c%i", c, r);
        GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c, NONE, buffer);
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
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c-1, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c-1, NONE, buffer);
            ct++;
          }

          if (c + 1 <= 'i')
          {
            sprintf(buffer, "%c%i", c+1, r);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c+1, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c+1, NONE, buffer);
            ct++;
          }
        }
        else
        {
          if (r - 1 > 0)
          {
            sprintf(buffer, "%c%i", c, r-1);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r-1, c, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r-1, c, NONE, buffer);
            ct++;
          }

          if (r + 1 <= SQUARES_SIZE)
          {
            sprintf(buffer, "%c%i", c, r+1);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r+1, c, NONE, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r+1, c, NONE, buffer);
            ct++;
          }
        }
      }
    }
    else
    {
      #ifdef DEBUG
        printf("Nobody there. Adding possible move...\n");
      #endif
      r = locToRow(neigh);
      c = 'a' + locToCol(neigh);
      sprintf(buffer, "%c%i", c, r);
      #ifdef DEBUG
        printf("r=%i, c=%c, b=%s\n", r, c, buffer);
      #endif
      GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c, NONE, strdup(buffer));
      // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c, NONE, buffer);
      ct++;
    }
  }

  ais->nextmoves.walk_size = ct;

  #ifdef DEBUG
    printf("done generating walking moves.\n");
  #endif
}


void
AIStage_generatePossibleMoves(aistage_t *ais)
{
  #ifdef DEBUG
    printf("Generating all possible moves.\n");
  #endif
  AIStage_generateWalkMoves(ais);

  int ct = 0;
  int r;
  char c;
  char buffer[] = "    ";
  for (int i = 0; i < SQUARES_SIZE_SQ; i++)
  {
    r = locToRow(i);
    c = 'a' + locToCol(i);

    if (r == SQUARES_SIZE) continue;
    if (c == 'i') continue;

    if (Board_validWall(&(ais->gamestate.board), HORIZONTAL, r, c))
    {
      sprintf(buffer, "%c%ih", c, r);
      GameMove_init(&(ais->nextmoves.moves[5+ct]), ais->gamestate.player, r, c, HORIZONTAL, strdup(buffer));
      // *(ais->nextmoves->wall_moves + ct) = GameMove_create(ais->gamestate->player, r, c, HORIZONTAL, buffer);
      ct++;
    }
    else
    {
      printf("Skipping wall %c%ih\n", c, r);
    }

    if (Board_validWall(&(ais->gamestate.board), VERTICAL, r, c))
    {
      sprintf(buffer, "%c%iv", c, r);
      GameMove_init(&(ais->nextmoves.moves[5+ct]), ais->gamestate.player, r, c, VERTICAL, strdup(buffer));
      // *(ais->nextmoves->wall_moves + ct) = GameMove_create(ais->gamestate->player, r, c, VERTICAL, buffer);
      ct++;
    }
    else
    {
      printf("Skipping wall %c%iv\n", c, r);
    }
  }

  ais->nextmoves.wall_size = ct;
}


void AIStage_updatePossibleMoves(aistage_t *ais)
{
  AIStage_generateWalkMoves(ais);

  gamemove_t *move;
  for (int i = 5 + ais->nextmoves.wall_size - 1; i >= 5; i--)
  {
    move = &(ais->nextmoves.moves[i]);
    if (!Board_validWall(&(ais->gamestate.board), move->wall, move->row, move->col))
    {
      ais->nextmoves.wall_size--;
      GameMove_clone(&(ais->nextmoves.moves[5 + ais->nextmoves.wall_size]), &(ais->nextmoves.moves[i]));
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

void
MoveSequence_init(moveseq_t *ms)
{
  // moveseq_t *bm = malloc(sizeof(moveseq_t));
  ms->num_moves = 0;
  ms->max_moves = 8;
  ms->score = BLOCKED_SCORE;
  // bm->moves = malloc(sizeof(gamemove_t *) * bm->max_moves);

  // return bm;
}

moveseq_t *
MoveSequence_clone(const moveseq_t *old, moveseq_t *ms)
{
  if (ms == NULL)
    ms = malloc(sizeof(moveseq_t));

  ms->num_moves = old->num_moves;
  ms->max_moves = old->max_moves;
  ms->score = old->score;
  // ms->moves = malloc(sizeof(gamemove_t *) * old->max_moves);

  for (int i = 0; i < old->num_moves; i++)
  {
    GameMove_clone(&(old->moves[i]), &(ms->moves[i]));
  }

  return ms;
}

bool
MoveSequence_add(moveseq_t *ms, gamemove_t *move)
{
  if (ms->num_moves == ms->max_moves)
  {
    return false;
  }

  GameMove_clone(move, &(ms->moves[ms->num_moves]));
  // *(ms->moves + ms->num_moves) = move;
  ms->num_moves++;
  return true;
}


void
MoveSequence_print(moveseq_t *ms)
{
  printf("(L=%i)", ms->num_moves);
  for (int i = 0; i < ms->num_moves; i++)
  {
    printf(",");
    GameMove_print(&(ms->moves[i]));
  }
  printf("\n");
}


void
BestMoves_init(bestmoves_t *bm)
{
  // bestmoves_t *bm = malloc(sizeof(bestmoves_t));
  bm->size = 0;
  bm->max_size = 32;
  bm->score = BLOCKED_SCORE + 1;
  bm->moves = malloc(sizeof(moveseq_t *) * (bm->max_size));

  // return bm;
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
BestMoves_print(bestmoves_t *bm)
{
  printf("Best score: %i\n", bm->score);
  for (int i = 0; i < bm->size; i++)
  {
    printf("\t");
    MoveSequence_print(*(bm->moves + i));
  }
}


void
BestMoves_destroy(bestmoves_t *bm)
{
  if (bm != NULL)
  {
    for (int i = 0; i < bm->size; i++)
    {
      // MoveSequence_destroy(*(bm->moves + i));
      *(bm->moves + i) = NULL;
    }
    free(bm->moves);
    free(bm);
  }
}

bestmoves_t
AIStage_bestMoves(aistage_t *ais, moveseq_t *ms, int lookahead)
{
  #ifdef DEBUGAI
    printf("Calculating best moves.\n");
    printf("Lookahead: %i\n", lookahead);
  #endif
  // odd lookahead is my turn, even is opponent's, but ignoring for now
  bestmoves_t best;
  BestMoves_init(&best);
  bestmoves_t results;
  moveseq_t newms;
  MoveSequence_init(&newms);

  aistage_t next;
  gamemove_t currmove;

  #ifdef DEBUGAI
    printf("Initialization done.\n");
  #endif

  for (int i = 0; i < 5 + ais->nextmoves.wall_size; i++)
  {
    #ifdef DEBUGAI
      printf("Loop index %i\n", i);
    #endif
    if (i < 5 && i >= ais->nextmoves.walk_size)
    {
      #ifdef DEBUGAI
        printf("Skipping due to missing walk move.\n");
      #endif
      continue;
    }

    GameMove_print(&(ais->nextmoves.moves[i]));
    printf("\n");

    GameMove_clone(&(ais->nextmoves.moves[i]), &currmove);
    GameMove_print(&currmove);
    printf("\n");

    #ifdef DEBUGAI
      printf("Advancing the game state.\n");
    #endif
    AIStage_advance(ais, &next, &currmove);
    #ifdef DEBUGAI
      printf("Advancing done.\n");
    #endif

    if (lookahead == 1)
    {
      #ifdef DEBUGAI
        printf("Lookahead 1!\n");
      #endif
      #ifdef DEBUGAI
        printf("Calculating stage score...\n");
      #endif
      AIStage_evaluateGameState(&next);
      #ifdef DEBUGAI
        printf("Killing bad results...\n");
      #endif
      if (next.stage_score > best.score)
      {
        best.score = next.stage_score; // now defined
        for (int k = best.size - 1; k >= 0 ; k--)
        {
          if ((*(best.moves + k))->score < best.score - BEST_TOLERANCE)
          {
            // MoveSequence_destroy(*(best.moves + k));
            best.size--;
            *(best.moves + k) = *(best.moves + best.size);
            *(best.moves + best.size) = NULL;
          }
        }
      }

      #ifdef DEBUGAI
        printf("Trying to add this result...\n");
      #endif

      if (next.stage_score >= best.score - BEST_TOLERANCE)
      {
        #ifdef DEBUGAI
          printf("It is a decent one...\n");
        #endif
        #ifdef DEBUGAI
          printf("Cloning move sequence...\n");
        #endif
        MoveSequence_clone(ms, &newms);
        #ifdef DEBUGAI
          printf("Adding this move...\n");
        #endif
        MoveSequence_add(&newms, &currmove);
        #ifdef DEBUGAI
          printf("Updating score...\n");
        #endif
        newms.score = next.stage_score;
        #ifdef DEBUGAI
          printf("Adding new best move...\n");
        #endif
        BestMoves_add(&best, &newms);
        #ifdef DEBUGAI
          printf("recording done.");
        #endif
      }
    }
    else if (lookahead % 2 == 0)
    {
      #ifdef DEBUGAI
        printf("Opponent lookahead!\n");
      #endif
      // like the else, but minimize instead of maximize
      MoveSequence_clone(ms, &newms);
      MoveSequence_add(&newms, &(ais->nextmoves.moves[i]));
      results = AIStage_bestMoves(&next, &newms, lookahead - 1);

      if (best.score > results.score) // results are far better
      {
        BestMoves_destroy(&best);
        best = results;
      }
      else if (results.score > best.score) //best is far better
      {
        BestMoves_destroy(&results);
      }
      else //mixed results
      {
        for (int b = 0; b < results.size; b++)
        {
          if ((*(results.moves + b))->score < best.score)
          {
            best.score = (*(results.moves + b))->score;
            for (int k = best.size - 1; k >= 0 ; k--)
            {
              if ((*(best.moves + k))->score > best.score)
              {
                best.size--;
                *(best.moves + k) = *(best.moves + best.size);
                *(best.moves + best.size) = NULL;
              }
            }
          }

          if ((*(results.moves + b))->score <= best.score)
          {
            BestMoves_add(&best, *(results.moves + b));
          }
        }
      }
    }
    else
    {
      #ifdef DEBUGAI
        printf("Odd lookahead!\n");
      #endif
      MoveSequence_clone(ms, &newms);
      MoveSequence_add(&newms, &(ais->nextmoves.moves[i]));
      results = AIStage_bestMoves(&next, &newms, lookahead - 1);

      if (best.score < results.score - BEST_TOLERANCE) // results are far better
      {
        BestMoves_destroy(&best);
        best = results;
      }
      else if (results.score < best.score - BEST_TOLERANCE) //best is far better
      {
        BestMoves_destroy(&results);
      }
      else //mixed results
      {
        for (int b = 0; b < results.size; b++)
        {
          if ((*(results.moves + b))->score > best.score)
          {
            best.score = (*(results.moves + b))->score;
            for (int k = best.size - 1; k >= 0 ; k--)
            {
              if ((*(best.moves + k))->score < best.score - BEST_TOLERANCE)
              {
                best.size--;
                *(best.moves + k) = *(best.moves + best.size);
                *(best.moves + best.size) = NULL;
              }
            }
          }

          if ((*(results.moves + b))->score >= best.score - BEST_TOLERANCE)
          {
            BestMoves_add(&best, *(results.moves + b));
          }
        }
      }
    }
  }

  return best;

}
