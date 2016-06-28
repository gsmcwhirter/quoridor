#include <stdio.h>
#include <stdbool.h>

#include "ai.h"
#include "board.h"
#include "gamestate.h"
#include "history.h"
#include "bestmoves.h"
#include "possmoves.h"
#include "search.h"
#include "move.h"

#define WALL_SCORE 3
#define PATH_SCORE 1
#define BEST_TOLERANCE 1


void
AIStage_init(aistage_t *ais, gamestate_t *gamestate) //, gamehistory_t *history)
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
  // #ifdef DEBUGAI
  //   printf("AIS Cloning history...\n");
  // #endif
  // GameHistory_print(history);
  // GameHistory_clone(history, &(ais->history));
  // #ifdef DEBUGAI
  //   printf("AIS done history.\n");
  // #endif
  #ifdef DEBUGAI
    printf("AIS Initializing possible moves...\n");
  #endif
  PossibleMoves_init(&(ais->nextmoves));
  #ifdef DEBUGAI
    printf("AIS done possible moves.\n");
  #endif
  ais->stage_score_defined = false;
}

aistage_t *
AIStage_advance(const aistage_t *oldais, aistage_t *ais, gamemove_t *move) //clone equiv
{
  if (ais == NULL)
  {
    ais = malloc(sizeof(aistage_t));
  }

  #ifdef DEBUGAI
    printf("Cloning state.\n");
  #endif
  GameState_clone(&(oldais->gamestate), &(ais->gamestate));
  #ifdef DEBUG
    printf("Advancing state.\n");
  #endif
  GameState_applyMove(&(ais->gamestate), move);
  // #ifdef DEBUGAI
  //   printf("Cloning history.\n");
  // #endif
  // GameHistory_clone(&(oldais->history), &(ais->history));
  // #ifdef DEBUGAI
  //   printf("Advancing history.\n");
  // #endif
  // GameHistory_push(&(ais->history), move, &(ais->gamestate));
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
  // char buffer[] = "    ";

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
        // sprintf(buffer, "%c%i", c, r);
        GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c, NONE);//, buffer);
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
            // sprintf(buffer, "%c%i", c-1, r);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c-1, NONE);//, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c-1, NONE, buffer);
            ct++;
          }

          if (c + 1 <= 'i')
          {
            //sprintf(buffer, "%c%i", c+1, r);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c+1, NONE);//, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r, c+1, NONE, buffer);
            ct++;
          }
        }
        else
        {
          if (r - 1 > 0)
          {
            //sprintf(buffer, "%c%i", c, r-1);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r-1, c, NONE);//, buffer);
            // *(ais->nextmoves->walk_moves + ct) = GameMove_create(ais->gamestate->player, r-1, c, NONE, buffer);
            ct++;
          }

          if (r + 1 <= SQUARES_SIZE)
          {
            //sprintf(buffer, "%c%i", c, r+1);
            GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r+1, c, NONE);//, buffer);
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
      // sprintf(buffer, "%c%i", c, r);
      #ifdef DEBUG
        printf("r=%i, c=%c, b=%s\n", r, c, buffer);
      #endif
      GameMove_init(&(ais->nextmoves.moves[ct]), ais->gamestate.player, r, c, NONE);//, strdup(buffer));
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
      GameMove_init(&(ais->nextmoves.moves[5+ct]), ais->gamestate.player, r, c, HORIZONTAL);//, strdup(buffer));
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
      GameMove_init(&(ais->nextmoves.moves[5+ct]), ais->gamestate.player, r, c, VERTICAL);//, strdup(buffer));
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

  Search_bfs_exists(&sr, &(ais->gamestate.board), PLAYER1, ais->gamestate.board.player1);
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
  Search_bfs_exists(&sr, &(ais->gamestate.board), PLAYER2, ais->gamestate.board.player2);
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
// AIStage_bestMoves(aistage_t *ais, bestmoves_t *best, gamehistory_t *history, moveseq_t *ms, int lookahead)
AIStage_bestMoves(aistage_t *ais, bestmoves_t *best, gamehistory_t *history, int lookahead)
{
  #ifdef DEBUGAI2
    printf("Calculating best moves for: ");
    printf("Lookahead: %i\n", lookahead);
  #endif
  // odd lookahead is my turn, even is opponent's, but ignoring for now
  bestmoves_t results;
  BestMoves_init(&results);
  // moveseq_t newms;
  // MoveSequence_init(&newms);

  aistage_t next;
  // AIStage_init(&next, &(ais->gamestate));
  gamemove_t *currmove;

  #ifdef DEBUGAI
    printf("Initialization done.\n");
  #endif

  for (int i = 0; i < 5 + ais->nextmoves.wall_size; i++)
  {
    BestMoves_reset(&results);
    #ifdef DEBUGAI
      printf("Loop index %i in lookahead %i\n", i, lookahead);
    #endif
    if (i < 5 && i >= ais->nextmoves.walk_size)
    {
      #ifdef DEBUGAI
        printf("Skipping due to missing walk move.\n");
      #endif
      continue;
    }
    else
    {
      #ifdef DEBUGAI
        printf("Checking move: ");
        GameMove_print(&(ais->nextmoves.moves[i]));
        printf("\n");
      #endif
    }

    // GameMove_clone(&(ais->nextmoves.moves[i]), &currmove);
    currmove = &(ais->nextmoves.moves[i]);

    #ifdef DEBUGAI
      printf("Advancing the game state.\n");
    #endif
    AIStage_advance(ais, &next, currmove);
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
      #ifdef DEBUGAI3
        printf("Best so far: %i\n", best->score);
        printf("Current score: %i\n", next.stage_score);
      #endif
      #ifdef DEBUGAI
        printf("Killing bad results...\n");
      #endif
      if (next.stage_score > best->score)
      {
        best->score = next.stage_score; // now defined
        for (int k = best->size - 1; k >= 0 ; k--)
        {
          if ((best->moves + k)->score < best->score - BEST_TOLERANCE)
          {
            // MoveSequence_destroy(*(best.moves + k));
            best->size--;
            // if (k != best->size)
            //   MoveSequence_clone((best->moves + best->size), (best->moves + k));
            // // *(best.moves + k) = *(best.moves + best.size);
            // // *(best.moves + best.size) = NULL;
          }
        }
      }

      #ifdef DEBUGAI
        printf("Trying to add this result...\n");
      #endif

      if (next.stage_score >= best->score - BEST_TOLERANCE)
      {
        #ifdef DEBUGAI
          printf("It is a decent one...\n");
        #endif
        // #ifdef DEBUGAI
        //   printf("Cloning move sequence...\n");
        // #endif
        // MoveSequence_clone(ms, &newms);
        // #ifdef DEBUGAI
        //   printf("Adding this move...\n");
        // #endif
        // MoveSequence_add(&newms, &currmove);
        #ifdef DEBUGAI
          printf("Updating score...\n");
        #endif
        currmove->score = next.stage_score;
        #ifdef DEBUGAI
          printf("Adding new best move...\n");
        #endif
        BestMoves_add(best, currmove, false);
        #ifdef DEBUGAI
          printf("recording done.");
        #endif
      }
      #ifdef DEBUGAI
      else
      {
        printf("Not good enough.\n");
      }
      #endif
    }
    else if (lookahead % 2 == 0)
    {
      #ifdef DEBUGAI
        printf("Opponent lookahead!\n");
      #endif
      // like the else, but minimize instead of maximize
      // MoveSequence_clone(ms, &newms);
      // MoveSequence_add(&newms, &(ais->nextmoves.moves[i]));
      // AIStage_bestMoves(&next, &results, history, &newms, lookahead - 1);
      AIStage_bestMoves(&next, &results, history, lookahead - 1);

      #ifdef DEBUGAI
        printf("results are in (size = %i, score = %i, current best = %i)\n", results.size, results.score, best->score);
      #endif

      currmove->score = results.score;

      if (best->score == BLOCKED_SCORE || (results.score > BLOCKED_SCORE && best->score > results.score)) // results are far "better" for opponent
      {
        #ifdef DEBUGAI
          printf("results are 'better' than the best! (r: %i vs b: %i)\n", results.score, best->score);
        #endif
        // BestMoves_clone(&results, best);
        // BestMoves_destroy(&results);
        BestMoves_reset(best);
        BestMoves_add(best, currmove, false);
        best->score = currmove->score;
      }
      else if (results.score > best->score && best->score > BLOCKED_SCORE) //best is far better
      {
        #ifdef DEBUGAI
          printf("best is 'better' than results (r: %i vs b: %i)\n", results.score, best->score);
        #endif
        // BestMoves_destroy(&results);
      }
      else //mixed results
      {
        #ifdef DEBUGAI
          printf("mixed results. (r: %i vs b: %i)\n", results.score, best->score);
        #endif
        for (int b = 0; b < results.size; b++)
        {
          if (results.moves[b].score > BLOCKED_SCORE && results.moves[b].score < best->score)
          {
            #ifdef DEBUGAI
              printf("results has a better item. (%i) cleaning up best.\n", results.moves[b].score);
            #endif
            best->score = results.moves[b].score;
            for (int k = best->size - 1; k >= 0 ; k--)
            {
              if (best->moves[k].score > best->score)
              {
                best->size--;
                // MoveSequence_clone((best->moves + best->size), (best->moves + k));
                // // *(best.moves + k) = *(best.moves + best.size);
                // // *(best.moves + best.size) = NULL;
              }
            }
          }

          if (results.moves[b].score > BLOCKED_SCORE && results.moves[b].score <= best->score)
          {
            currmove->score = results.moves[b].score;
            #ifdef DEBUGAI
              printf("adding current result to best (%i)\n", results.moves[b].score);
            #endif
            // BestMoves_add(best, &(results.moves[b]), false);
            BestMoves_add(best, currmove, false);
            #ifdef DEBUGAI
              printf("added.\n");
            #endif
          }
          #ifdef DEBUGAI
          else
          {
            printf("current result wasn't good enough\n");
          }
          #endif
        }
      }
    }
    else
    {
      #ifdef DEBUGAI
        printf("Odd lookahead!\n");
      #endif
      // MoveSequence_clone(ms, &newms);
      // MoveSequence_add(&newms, &(ais->nextmoves.moves[i]));
      // AIStage_bestMoves(&next, &results, history, &newms, lookahead - 1);
      AIStage_bestMoves(&next, &results, history, lookahead - 1);

      currmove->score = results.score;

      if (best->score == BLOCKED_SCORE || best->score < results.score - BEST_TOLERANCE) // results are far better
      {
        // BestMoves_clone(&results, best);
        BestMoves_reset(best);
        BestMoves_add(best, currmove, false);
        best->score = currmove->score;
        // BestMoves_destroy(&results);
      }
      else if (results.score < best->score - BEST_TOLERANCE) //best is far better
      {
        // BestMoves_destroy(&results);
      }
      else //mixed results
      {
        for (int b = 0; b < results.size; b++)
        {
          if (results.moves[b].score > best->score)
          {
            best->score = results.moves[b].score;
            for (int k = best->size - 1; k >= 0 ; k--)
            {
              if (best->moves[k].score < best->score - BEST_TOLERANCE)
              {
                best->size--;
                // MoveSequence_clone((best->moves + best->size), (best->moves + k));
                // // *(best.moves + k) = *(best.moves + best.size);
                // // *(best.moves + best.size) = NULL;
              }
            }
          }

          if (results.moves[b].score >= best->score - BEST_TOLERANCE)
          {
            currmove->score = results.moves[b].score;
            // BestMoves_add(best, &(results.moves[b]), false);
            BestMoves_add(best, currmove, false);
          }
        }
      }
    }

    #ifdef DEBUGAI
      printf("Finished loop index %i in lookahead %i\n", i, lookahead);
    #endif
  }

  #ifdef DEBUGAI2
    printf("Done one BestMoves call for: ");
    printf("Lookahead: %i\n", lookahead);
    printf("\tBest score: %i\n", best->score);
  #endif

  // BestMoves_destroy(&results);

  // return best;

}
