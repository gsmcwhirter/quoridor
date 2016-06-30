#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "ai.h"
#include "board.h"
#include "gamestate.h"
#include "history.h"
#include "bestmoves.h"
#include "possmoves.h"
#include "search.h"
#include "move.h"

#define WALL_SCORE 1
#define PATH_SCORE 1
#define BEST_TOLERANCE 0
#define CALCULATION_TIMEOUT_SEC 10
#define CALCULATION_TIMEOUT_NSEC 0

pthread_mutex_t calculating = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;

void
AIStage_init(aistage_t *ais, gamestate_t *gamestate) //, gamehistory_t *history)
{
  #ifdef DEBUGAI
    printf("AIS Init Setting player... (%i)\n", gamestate->player);
  #endif
  ais->my_player = gamestate->player;
  #ifdef DEBUGAI
    if (ais->my_player == PLAYER1) printf("\tPlayer 1\n");
    else printf("\tPlayer 2\n");
    printf("AIS Init Cloning state...\n");
  #endif
  GameState_clone(gamestate, &(ais->gamestate));
  #ifdef DEBUGAI
    printf("AIS Init done state.\n");
  #endif
  #ifdef DEBUGAI
    printf("AIS Init Initializing possible moves...\n");
  #endif
  PossibleMoves_init(&(ais->nextmoves));
  #ifdef DEBUGAI
    printf("AIS Init done possible moves.\n");
  #endif
  ais->stage_score_defined = false;
}

aistage_t *
AIStage_advance(const aistage_t *oldais, aistage_t *ais, gamemove_t *move) //clone equiv
{
  #ifdef DEBUGAI
    printf("AIS Advance starting.\n");
  #endif
  if (ais == NULL)
  {
    ais = malloc(sizeof(aistage_t));
  }

  ais->my_player = oldais->my_player;

  #ifdef DEBUGAI
    printf("AIS Advance Cloning state.\n");
  #endif
  GameState_clone(&(oldais->gamestate), &(ais->gamestate));
  #ifdef DEBUGAI
    printf("AIS Advance Advancing state.\n");
    printf("\tPlaying move:");
    GameMove_print(move);
    printf("\n");
  #endif
  moveresult_t res = GameState_applyMove(&(ais->gamestate), move, true);
  if (res != OK)
  {
    // printf("MOVE NOT APPLIED!\n");
    // GameMove_print(move);
    // printf("\n");
    GameState_print(&(ais->gamestate), PLAYER1);
    // printf("\t%s\n", moveDescription(res));
  }
  GameState_togglePlayer(&(ais->gamestate));
  #ifdef DEBUGAI
    printf("AIS Advance Cloning and updating possible moves.\n");
  #endif
  PossibleMoves_clone(&(oldais->nextmoves), &(ais->nextmoves));
  AIStage_updatePossibleMoves(ais);
  ais->stage_score_defined = false;

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
AIStage_generateWallMoves(aistage_t *ais)
{
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
    #ifdef DEBUGAI
    else
    {
      printf("Skipping wall %c%ih\n", c, r);
    }
    #endif

    if (Board_validWall(&(ais->gamestate.board), VERTICAL, r, c))
    {
      sprintf(buffer, "%c%iv", c, r);
      GameMove_init(&(ais->nextmoves.moves[5+ct]), ais->gamestate.player, r, c, VERTICAL);//, strdup(buffer));
      // *(ais->nextmoves->wall_moves + ct) = GameMove_create(ais->gamestate->player, r, c, VERTICAL, buffer);
      ct++;
    }
    #ifdef DEBUGAI
    else
    {
      printf("Skipping wall %c%iv\n", c, r);
    }
    #endif
  }

  ais->nextmoves.wall_size = ct;
}


void
AIStage_generatePossibleMoves(aistage_t *ais)
{
  #ifdef DEBUG
    printf("Generating all possible moves.\n");
  #endif
  AIStage_generateWalkMoves(ais);

  AIStage_generateWallMoves(ais);
}


void AIStage_updatePossibleMoves(aistage_t *ais)
{
  AIStage_generateWalkMoves(ais);

  gamemove_t *move;
  // if ((ais->gamestate.player == PLAYER1 && ais->gamestate.player1_walls == 0) ||
  //     (ais->gamestate.player == PLAYER2 && ais->gamestate.player2_walls == 0))
  // {
  //   ais->nextmoves.wall_size = 0;
  //   return;
  // }
  // else if (ais->nextmoves.wall_size == 0)
  // {
  //     AIStage_generateWallMoves(ais);
  //     return;
  // }

  for (int i = 5 + ais->nextmoves.wall_size - 1; i >= 5; i--)
  {
    move = &(ais->nextmoves.moves[i]);
    move->player = ais->gamestate.player;
    if (!Board_validWall(&(ais->gamestate.board), move->wall, move->row, move->col))
    {
      ais->nextmoves.wall_size--;
      GameMove_clone(&(ais->nextmoves.moves[5 + ais->nextmoves.wall_size]), &(ais->nextmoves.moves[i]));
    }
  }
}

int
AIStage_evaluateGameState(aistage_t *ais, gamehistory_t *history)
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

  if (ais->gamestate.board.player1 >= PLAYER1_TARGET)
  {
    ais->stage_score_defined = true;
    if (ais->my_player == PLAYER1)
    {
      ais->stage_score = WIN_SCORE;
      return WIN_SCORE;
    }
    else
    {
      ais->stage_score = -WIN_SCORE;
      return -WIN_SCORE;
    }
  }
  else if (ais->gamestate.board.player2 <= PLAYER2_TARGET)
  {
    ais->stage_score_defined = true;
    if (ais->my_player == PLAYER2)
    {
      ais->stage_score = WIN_SCORE;
      return WIN_SCORE;
    }
    else
    {
      ais->stage_score = -WIN_SCORE;
      return -WIN_SCORE;
    }
  }

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
    // p1pathlen = sr.shortest_paths[0].length;
    p1pathlen = PATH_SCORE * sr.shortest_length;
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
    // p2pathlen = p1pathlen = sr.shortest_paths[0].length;;
    p2pathlen = PATH_SCORE * sr.shortest_length;
  }

  #ifdef DEBUGAI
    printf("State Evaluation:\n");
    printf("\tP1 Path Length: %i\n", p1pathlen);
    printf("\tP2 Path Length: %i\n", p2pathlen);
    printf("\tP1 Wall Value: %i\n", p1walls);
    printf("\tP2 Wall Value: %i\n", p2walls);
  #endif


  if (ais->my_player == PLAYER1) //player 1 just played
  {
    score = p1walls - p2walls + p2pathlen - p1pathlen;
    #ifdef DEBUGAI
      printf("\t(calculated for Player 1, got %i)\n", score);
    #endif
  }
  else
  {
    score = p2walls - p1walls + p1pathlen - p2pathlen;
    #ifdef DEBUGAI
      printf("\t(calculated for Player 2, got %i)\n", score);
    #endif
  }

  ais->stage_score_defined = true;
  ais->stage_score = score;
  return score;
}


void
minimax_minimize(bestmoves_t *best, gamemove_t *move)
{
  #ifdef DEBUGAI
    printf("Minimize: starting\n");
  #endif
  if (best->score == BLOCKED_SCORE || move->score < best->score)
  {
    #ifdef DEBUGAI
      printf("\tNew score is 'better' or no old score.\n");
      printf("\t(old: %i, this: %i)\n", best->score, move->score);
    #endif
    best->score = move->score;
    #ifdef DEBUGAI
      printf("\tCleaning up existing moves\n");
    #endif
    for (int i = best->size - 1; i >= 0; i--)
    {
      if (best->moves[i].score > best->score + BEST_TOLERANCE)
      {
        best->size--;
      }
    }
    #ifdef DEBUGAI
      printf("\tAdding this move.\n");
    #endif
    BestMoves_add(best, move, false);
  }
  else if (move->score > BLOCKED_SCORE && move->score <= best->score + BEST_TOLERANCE)
  {
    #ifdef DEBUGAI
      printf("\tNew score is not 'worse'.\n");
      printf("\t(old: %i, this: %i)\n", best->score, move->score);
      printf("\tAdding this move.\n");
    #endif
    BestMoves_add(best, move, false);
  }
  #ifdef DEBUGAI
  else
  {
    printf("\tNew score is 'worse'.\n");
    printf("\t(old: %i, this: %i)\n", best->score, move->score);
  }
  #endif
}

void
minimax_maximize(bestmoves_t *best, gamemove_t *move)
{
  #ifdef DEBUGAI
    printf("Maximize: starting\n");
  #endif
  if (best->score == BLOCKED_SCORE || move->score > best->score)
  {
    #ifdef DEBUGAI
      printf("\tNew score is 'better' or no old score.\n");
      printf("\t(old: %i, this: %i)\n", best->score, move->score);
    #endif
    best->score = move->score;
    #ifdef DEBUGAI
      printf("\tCleaning up existing moves\n");
    #endif
    for (int i = best->size - 1; i >= 0; i--)
    {
      if (best->moves[i].score < best->score - BEST_TOLERANCE)
      {
        best->size--;
      }
    }
    #ifdef DEBUGAI
      printf("\tAdding this move.\n");
    #endif
    BestMoves_add(best, move, true);
  }
  else if (move->score > BLOCKED_SCORE && move->score >= best->score - BEST_TOLERANCE)
  {
    #ifdef DEBUGAI
      printf("\tNew score is not 'worse'.\n");
      printf("\t(old: %i, this: %i)\n", best->score, move->score);
      printf("\tAdding this move.\n");
    #endif
    BestMoves_add(best, move, true);
  }
  #ifdef DEBUGAI
  else
  {
    printf("\tNew score is 'worse'.\n");
    printf("\t(old: %i, this: %i)\n", best->score, move->score);
  }
  #endif
}

void
minimax(aistage_t *ais, bestmoves_t *best, gamehistory_t *history, gamemove_t *move, int lookahead)
{
  #ifdef DEBUGAI
    printf("Starting minimax: ");
    printf("Lookahead %i!\n", lookahead);
  #endif

  aistage_t next;
  gamemove_t currmove;

  bestmoves_t results;
  BestMoves_init(&results);

  if (lookahead == 0 || GameState_isGameOver(&(ais->gamestate)))
  {
    #ifdef DEBUGAI
      printf("Minimax: evaluating state\n");
    #endif
    AIStage_evaluateGameState(ais, history);
    #ifdef DEBUGAI
      printf("Minimax: setting move score\n");
    #endif
    move->score = ais->stage_score;
    #ifdef DEBUGAI
      printf("Minimax: minimizing\n");
    #endif
    minimax_minimize(best, move);
  }
  else
  {
    for (int i = 0; i < 5 + ais->nextmoves.wall_size; i++)
    {
      if (i < 5 && i >= ais->nextmoves.walk_size)
      {
        continue;
      }
      GameMove_clone(&(ais->nextmoves.moves[i]), &currmove);
      AIStage_advance(ais, &next, &currmove);
      minimax(&next, &results, history, &currmove, lookahead-1);

      currmove.score = results.score;
      if (lookahead % 2 == 1)
        minimax_maximize(best, &currmove);
      else
        minimax_minimize(best, &currmove);
    }
  }
}

void
alphabeta(aistage_t *ais, bestmoves_t *best, gamehistory_t *history, gamemove_t *move, int lookahead, int alpha, int beta, bool maximizer)
{
  pthread_testcancel();
  #ifdef DEBUGAI
    printf("Starting alphabeta: ");
    printf("Lookahead %i!\n", lookahead);
    printf("alpha=%i, beta=%i\n", alpha, beta);
  #endif

  aistage_t next;
  gamemove_t *currmove;

  bestmoves_t results;
  BestMoves_init(&results);

  if (lookahead == 0 || GameState_isGameOver(&(ais->gamestate)))
  {
    #ifdef DEBUGAI
      printf("Alphabeta: evaluating state\n");
    #endif
    AIStage_evaluateGameState(ais, history);
    #ifdef DEBUGAI
      printf("Alphabeta: setting move score\n");
    #endif
    move->score = ais->stage_score;
    #ifdef DEBUGAI
      printf("Alphabeta: minimizing\n");
    #endif
    // if (maximizer)
    // {
    //   minimax_maximize(best, move);
    // }
    // else
    // {
    //   minimax_minimize(best, move);
    // }
    best->score = move->score;
    BestMoves_add(best, move, false);
  }
  else
  {
    #ifdef DEBUGAI
      printf("Alphabeta: Considering possible moves.\n");
    #endif
    for (int i = 0; i < 5 + ais->nextmoves.wall_size; i++)
    {
      BestMoves_reset(&results);
      if (i < 5 && i >= ais->nextmoves.walk_size)
      {
        continue;
      }

      if (i >= 5 &&
          ((ais->gamestate.player == PLAYER1 && ais->gamestate.player1_walls == 0) ||
           (ais->gamestate.player == PLAYER2 && ais->gamestate.player2_walls == 0)))
      {
        break;
      }
      // GameMove_clone(&(ais->nextmoves.moves[i]), &currmove);
      currmove = &(ais->nextmoves.moves[i]);
      #ifdef DEBUGAI
        printf("\tMove under consideration:\n");
        GameMove_print(currmove);
        printf("\n");
        printf("\tAdvancing stage.\n");
      #endif
      AIStage_advance(ais, &next, currmove);
      #ifdef DEBUGAI
        printf("\trecursing with alpha=%i, beta=%i.\n", alpha, beta);
      #endif
      alphabeta(&next, &results, history, currmove, lookahead-1, alpha, beta, maximizer ? false : true);

      #ifdef DEBUGAI
        printf("Alphabeta: returned from recursion.\n");
      #endif
      currmove->score = results.score;
      #ifdef DEBUGAI
        printf("\tScore of this move: %i\n", currmove->score);
      #endif

      // if (lookahead % 2 == 0)
      if (maximizer)
      {
        #ifdef DEBUGAI
          printf("\tI am the current player, so maximizing.\n");
        #endif
        alpha = (alpha > currmove->score) ? alpha : currmove->score;
        minimax_maximize(best, currmove);
        if (beta <= alpha)
        {
          #ifdef DEBUGAI
            printf("\tTerminated loop early.\n");
          #endif
          break;
        }
      }
      else
      {
        #ifdef DEBUGAI
          printf("\tI am NOT the current player, so minimizing.\n");
        #endif
        beta = (beta < currmove->score) ? beta : currmove->score;
        minimax_minimize(best, currmove);
        if (beta <= alpha)
        {
          #ifdef DEBUGAI
            printf("\tTerminated loop early.\n");
          #endif
          break;
        }
      }
    }
  }
}

void
_bestMove(aistage_t *ais, gamemove_t **bestmove, gamehistory_t *history, int lookahead)
{
  #ifdef DEBUGAI
    printf("Calculating best moves for player %i\n", ais->my_player);
    printf("Looking ahead %i moves\n", lookahead);
  #endif
  bestmoves_t best;
  BestMoves_init(&best);
  // minimax(ais, best, history, NULL, levels * 2, ais->my_player);
  alphabeta(ais, &best, history, NULL, lookahead, BLOCKED_SCORE - 1, -BLOCKED_SCORE + 1, true);

  if (best.size == 0)
  {
    printf("Error: no best move?\n");
    *(bestmove) = NULL;
  }
  else if (best.size == 1)
  {
    printf("Found one best move.\n");
    GameMove_clone(&(best.moves[0]), *bestmove);
  }
  else
  {
    printf("Selecting a random move from %i best moves.\n", best.size);
    srand((unsigned int)time(NULL));
    GameMove_clone(&(best.moves[rand() % best.size]), *bestmove);
  }
}

void *
bestMoveThread(void *data)
{
  int oldtype1;
  int oldtype2;
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldtype1);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype2);
  aithreadarg_t *args = (aithreadarg_t *)data;

  int lookahead = 3;
  while (true)
  {
    pthread_testcancel();
    _bestMove(args->ais, &(args->bestmove), args->history, lookahead);
    lookahead += 2;
    *(args->last_lookahead) = lookahead;
    if (args->bestmove == NULL) break;
  }
  pthread_cond_signal(&done);
  return NULL;
}

void
AIStage_bestMove(aistage_t *ais, gamehistory_t *history, gamemove_t *bestmove, int *lookahead)
{
  struct timespec max_wait;
  memset(&max_wait, 0, sizeof(max_wait));

  /* wait at most 2 seconds */
  max_wait.tv_sec = CALCULATION_TIMEOUT_SEC;
  max_wait.tv_nsec = CALCULATION_TIMEOUT_NSEC;

  struct timespec abs_time;
  pthread_t tid;
  int err;

  pthread_mutex_lock(&calculating);

  // OS X workaround: http://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
  /* pthread cond_timedwait expects an absolute time to wait until */
  #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  abs_time.tv_sec = mts.tv_sec;
  abs_time.tv_nsec = mts.tv_nsec;

  #else
  clock_gettime(CLOCK_REALTIME, &abs_time);
  #endif
  abs_time.tv_sec += max_wait.tv_sec;
  abs_time.tv_nsec += max_wait.tv_nsec;

  aithreadarg_t threadarg;
  threadarg.ais = ais;
  threadarg.history = history;
  threadarg.bestmove = bestmove;
  threadarg.last_lookahead = lookahead;

  printf("Creating thread.\n");
  pthread_create(&tid, NULL, bestMoveThread, &threadarg);

  while (true)
  {
    err = pthread_cond_timedwait(&done, &calculating, &abs_time);

    if (err == ETIMEDOUT)
    {
      fprintf(stderr, "%s: calculation timed out\n", __func__);
      printf("Timeout done.\n");
      pthread_cancel(tid);
      break;
    }
    else if (err == 0)
    {
      break;
    }
  }

  void * t = NULL;

  pthread_join(tid, &t);
  pthread_mutex_unlock(&calculating);


}
