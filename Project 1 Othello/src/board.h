/*Donghyun Park
Prof. Sable
Artificial Intelligence: Othello
11/5/17*/

#ifndef _BOARD_H
#define _BOARD_H

#include <vector>
#include "properties.h"

using namespace std;

class Board{
public:

  /*typedef struct{
    char x,y;
  } Square;

  typedef struct{

  } Move;*/
  class boardSquare{
  public:
    char x, y;
    boardSquare();  //overloaded constructor, used when no parameters are passed
    boardSquare(char x, char y);
  };

  class boardMove{
  public:
    boardMove();
    boardMove(char x, char y);
    bool validMove;
    Board::boardSquare square;
    vector<boardSquare> flip;
  };

  int currentPlayer;
  int score[3];
  char board[8][8];
  bool playerPassed;

  Board();
  Board(Board &bo);
  Board(char state[8][8], int currentPlayer);

  void print(vector<Board::boardMove> moves = vector<Board::boardMove>(), bool isComputer = false);
  bool checkFrontier(int x, int y);
  void makeMove(Board::boardMove chosenMove);
  bool checkEnd(bool passCurrentPlayer);
  bool nextPlayer(bool passCurrentPlayer);
  void gameOver();

  vector<Board::boardMove> discoverMoves(int player);

private:
  bool checkOnBoard(const char x, const char y);
  bool iterate(char &x, char &y, const int mode, const int direction);
};

#endif //_BOARD_H
