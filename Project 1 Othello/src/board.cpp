/*Donghyun Park
Prof. Sable
Artificial Intelligence: Othello
11/5/17*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include "board.h"
#include "properties.h"
#include <iomanip>

using namespace std;

const int gameModes[4] = {1, 2, 3, 4};
const int gameDirections[2] = {-1, 1};

Board::boardSquare::boardSquare(){} //empty square constructor

Board::boardSquare::boardSquare(char x, char y){  //square constructors with initial coordinate parameters
  this->x = x;
  this->y = y;
}

Board::boardMove::boardMove(){} //empty move constructor

Board::boardMove::boardMove(char x, char y){  //available moves aggregator
  this->square.x = x;
  this->square.y = y;
  this->validMove = false;
}

Board::Board(){ //board constructor, creates a new board
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      //row value = y-axis, column value = x-axis
      board[i][j] = ' ';
    }
  }
  //initial config
  board[3][4] = white;
  board[4][3] = white;
  board[3][3] = black;
  board[4][4] = black;
  score[black] = 2;
  score[white] = 2;

  currentPlayer = black;
  playerPassed = false;
}

Board::Board(Board &bo){ //copy constructor, copies the board state from a referenced board for AI search purposes
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      this->board[i][j] = bo.board[i][j];
    }
  }
  for(int i = 0; i < 2; i++){
    this->score[i] = bo.score[i];
  }

  this->currentPlayer = bo.currentPlayer;
  this->playerPassed = bo.playerPassed;
}

Board::Board(char state[8][8], int currentPlayer){  //board constructor, initialized with a given state and player turn. Import board
  score[black] = 0;
  score[white] = 0;

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = state[i][j];
      if(state[i][j] == black){
        score[black]++;
      }else if(state[i][j] == white){
        score[white]++;
      }
    }
  }

  this->currentPlayer = currentPlayer;
  playerPassed = false;
}

void Board::print(vector<Board::boardMove> moves, bool isComputer){ //printing the board onto output
  cout << endl << "    0   1   2   3   4   5   6   7" << endl;
  cout << "  ---------------------------------" << endl;
  for(int i = 0; i < 8; i++){
    cout << i << " |";
    for(int j = 0; j < 8; j++){
      bool moveOptions = false;
      for(int k = 0; k < moves.size(); k++){
        if(moves[k].square.y == i && moves[k].square.x == j){
          if(isComputer){
            cout << yellow << setw(2) << " AI" << reset << setw(2) << "|";
          }else{
            cout << green << setw(2) << (char)(k+65) << reset << setw(2) << "|";
          }
          moveOptions = true;
        }
      }
      if(!moveOptions){
        if(board[i][j] == white){
          cout << red;
          cout << setw(2) << (int)board[i][j] << reset << setw(2) << "|";
        }else if(board[i][j] == black){
          cout << cyan;
          cout << setw(2) << (int)board[i][j] << reset << setw(2) << "|";
        }else{
          cout << setw(2) << (char)board[i][j] << reset << setw(2) << "|";
        }
      }
    }
    cout << endl;
    cout << "  ---------------------------------" << endl;
  }
  cout << endl;
}

bool Board::checkFrontier(int x, int y){ //checking for a piece existence on a frontier
  if(board[y][x] == ' '){
    return false;
  }
  for(int i = 0; i < modes; i++){
    int mode = gameModes[i];
    for(int j = 0; j < boardDirection; j++){
      int direction = gameDirections[j];
      char xDirect = x;
      char yDirect = y;
      iterate(xDirect, yDirect, mode, direction);
      if(checkOnBoard(xDirect, yDirect)){
        if(board[yDirect][xDirect] != ' '){
          return true;
        }
      }
    }
  }
  return false;
}

void Board::makeMove(Board::boardMove chosenMove){  //apply a chosen move and flip pieces accordingly
  //[row = y-axis][column = x-axis]
  board[chosenMove.square.y][chosenMove.square.x] = currentPlayer;

  if(currentPlayer == black){
    score[black]++;
  }else{
    score[white]++;
  }

  for(int i = 0; i < chosenMove.flip.size(); i++){
    board[chosenMove.flip[i].y][chosenMove.flip[i].x] = currentPlayer;
    if(currentPlayer == black){
      score[black]++;
      score[white]--;
    }else{
      score[black]--;
      score[white]++;
    }
  }
}

bool Board::nextPlayer(bool passCurrentPlayer){  //change player
  if(checkEnd(passCurrentPlayer)){
    return true;
  }
  currentPlayer = (currentPlayer == white) ? black : white;
  playerPassed = false;
  return false;
}

bool Board::checkEnd(bool passCurrentPlayer){  //check for the endgame
  if((score[black] + score[white] == squares) || (playerPassed && passCurrentPlayer)){
    return true;
  }else{
    return false;
  }
}

void Board::gameOver(){  //endgame display
  cout << "End of game" << endl;
  cout << "Black: " << score[black] << " points" << endl;
  cout << "White: " << score[white] << " points" << endl;
  if(score[black] > score[white]){
    cout << "Black wins the game." << endl;
  }else if(score[black] < score[white]){
    cout << "White wins the game." << endl;
  }else if(score[black] == score[white]){
    cout << "The game has ended in a tie." << endl;
  }
}

vector<Board::boardMove> Board::discoverMoves(int player){ //check for legal moves a player can makeMove
  vector<Board::boardMove> moveList;

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(board[i][j] != ' '){ //j = x axis, i = y axis
        continue; //skip occupied spaces
      }
      Board::boardMove available = Board::boardMove(j,i); //keep track of nonoccupied spaces

      for(int k = 0; k < modes; k++){
        int mode = gameModes[k];

        for(int l = 0; l < boardDirection; l++){
          int direction = gameDirections[l];
          char x = available.square.x;
          char y = available.square.y;

          vector<Board::boardSquare> follow;  //keep track of pieces that can be flipped
          iterate(x, y, mode, direction);

          if((board[y][x] == ' ') || (board[y][x] == player)){
            continue; //check if opponent's piece is occupying the space
          }

          for(x, y; checkOnBoard(x, y); iterate(x, y, mode, direction)){
            if(board[y][x] == player){
              available.validMove = true;
              available.flip.insert(available.flip.end(), follow.begin(), follow.end());
              break;
            }else if(board[y][x] == ' '){
              break;
            }else{
              follow.push_back(Board::boardSquare(x, y));
            }
          }
        }
      }
      if(available.validMove){
        moveList.push_back(available);
      }
    }
  }
  return moveList;
}

//private methods below

bool Board::checkOnBoard(const char x, const char y){
  return (x >= 0) && (x < 8) && (y >= 0) && (y < 8);
}

bool Board::iterate(char &x, char &y, const int mode, const int direction){ //iterate through the surrounding squares
  if((direction != 1) && (direction != -1)){
    cout << "iteration increment not valid: " << direction << endl;
    return false;
  }

  switch(mode){
    case 1: //vertical iteration
      y += direction;
      return true;
      break;

    case 2: //horizontal iteration
      x += direction;
      return true;
      break;

    case 3: //right diagonal
      x += direction;
      y += direction;
      return true;
      break;

    case 4: //left diagonal
      x -= direction;
      y += direction;
      return true;

    default:
      cout << "tried iterating in invalid direction: " << mode << endl;
      return false;
  }
}
