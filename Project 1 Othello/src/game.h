#ifndef _GAME_H
#define _GAME_H

#include <time.h>
#include "board.h"
#include "properties.h"

using namespace std;

class Game{
public:
  Game(); //overloaded constructor
  void setGame(int type);
  void playGame();

private:
  Board board;

  int tLimit;
  clock_t tStart;
  bool noTime;
  int playerMax;
  bool humanPlayer[3];

  int heuristic(Board board);
  int alphaBeta(Board board, int depth, int alpha, int beta, bool playerMax);
  bool humanMove();
  bool aiMove();
  bool randomMove();
};

#endif //_GAME_H
