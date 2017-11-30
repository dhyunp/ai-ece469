/*Donghyun Park
Prof. Sable
Artificial Intelligence: Othello
11/5/17*/

#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "board.h"
#include "game.h"
#include "properties.h"

using namespace std;

Game::Game(){}  //overloaded constructors

void Game::setGame(int type){  //set up the game board and state of game depending on new game or imported
  switch(type){
    case 1:
    //if the player goes first, he/she is assigned to black pieces
      humanPlayer[black] = true;
      humanPlayer[white] = false;
      break;

    case 2:
    //if the player goes second, he/she is assigned to white pieces
      humanPlayer[black] = false;
      humanPlayer[white] = true;
      break;

    case 3:
    //ai vs ai
      humanPlayer[black] = false;
      humanPlayer[white] = false;
      break;
  }
  char saveFile;

  cout << "Import a saved game? (y/n): ";
  cin >> saveFile;

  if(tolower(saveFile) == 'y'){ //importing and initialize board accordingly
    char boardSave[8][8];
    char x;
    int player;
    int i = 0;
    int j = 0;
    string saveName;
    fstream boardFile;

    cout << "Save file name: ";
    cin >> saveName;

    boardFile.open(saveName.c_str(), fstream::in);

    while(boardFile.fail()){
      cout << "Please enter a valid save file name: ";
      cin >> saveName;
      boardFile.open(saveName.c_str(), fstream::in);
    }

    while((x = boardFile.get()) != EOF){  //given that 0 = empty 1 = black 2 = white
      if((x == '1') || (x == '2')){
        boardSave[i][j++] = x-'0';
        if(j == 8){
          j = 0;
          if(++i == 8){
            break;
          }
        }
      }else if(x == '0'){
        boardSave[i][j++] = ' ';
      }
    }

    boardFile >> player;
    boardFile >> tLimit;
    board = Board(boardSave, player); //initialize imported board state

    boardFile.close();
  }else if(tolower(saveFile) == 'n'){
    string setTime;
    tLimit = -1;
    while(tLimit < 0){
      cout << "Enter time limit [5-60s]: ";
      cin >> setTime;
      if(isdigit(setTime.c_str()[0])){
        tLimit = atoi(setTime.c_str());
      }
    }
    board = Board();  //initialize empty board
  }else{
    cout << "Please answer y(yes) or n(no)" << endl;
  }
}

void Game::playGame(){  //game maintaining loop
  bool endGame = false;
  srand(time(NULL));

  cout << endl << "Starting game..." << endl;

  board.print();

  while(!endGame){
    if(humanPlayer[board.currentPlayer]){
      endGame = humanMove();
    }else{
      endGame = aiMove();
    }
  }
  board.print();
  board.gameOver();
}

//private methods below

int Game::heuristic(Board board){ //heuristic function for Othello
  int player, opposite, opponent;
  double pieceNum, edge, frontier, corner, maybeCorner, mobility;

  opponent = ((playerMax == white) ? black : white);

  pieceNum = ((100.0*board.score[playerMax]) / (board.score[playerMax] + board.score[opponent]));

  int pieceWeight = 2*(board.score[playerMax] + board.score[opponent]);
  int frontierWeight = 300;
  int edgeWeight = 70;
  int cornerWeight = 6000;
  int maybeCornerWeight = 3000;
  int mobilityWeight = 10*(100-(board.score[playerMax] + board.score[opponent]));

  player = 0;
  opposite = 0;

  if(board.board[0][0] == playerMax){ //corners
    player++;
  }else if(board.board[0][0] == opponent){
    opposite++;
  }
  if(board.board[0][7] == playerMax){
    player++;
  }else if(board.board[0][7] == opponent){
    opposite++;
  }
  if(board.board[7][0] == playerMax){
    player++;
  }else if(board.board[7][0] == opponent){
    opposite++;
  }
  if(board.board[7][7] == playerMax){
    player++;
  }else if(board.board[7][7] == opponent){
    opposite++;
  }

  corner = 25.0*(player - opposite);

  int myEdges = 0;
  int oppEdges = 0;
  int myFrontier = 0;
  int oppFrontier = 0;

  for(int i = 0; i < 8; i++){ //edges&fronter
    for(int j = 0; j < 8; j++){
      if(i == 0 || i == 7 || j == 0 || j == 7){
        if(board.board[i][j] == playerMax){
          myEdges++;
        }else if(board.board[i][j] == opponent){
          oppEdges++;
        }
      }else if(board.board[i][j] != ' '){
        if(board.checkFrontier(j, i)){  //i = row = y-axis j = column = x-asix
          if(board.board[i][j] == playerMax){
            myFrontier++;
          }else if(board.board[i][j] == opponent){
            oppFrontier++;
          }
        }
      }
    }
  }

  edge = 100.0*myEdges/(myEdges + oppEdges);
  frontier = -100*(myFrontier - oppFrontier);

  vector<Board::boardMove> playerLegal = board.discoverMoves(playerMax);
  vector<Board::boardMove> opponentLegal = board.discoverMoves(opponent);
  mobility = 100.0*playerLegal.size()/(playerLegal.size() + opponentLegal.size());

  //checking for potential corners
  opposite = 0;
  for(int i = 0; i < opponentLegal.size(); i++){
    if(opponentLegal[i].square.x == 0 && opponentLegal[i].square.y == 0){
      opposite++;
    }else if(opponentLegal[i].square.x == 7 && opponentLegal[i].square.y == 0){
      opposite++;
    }else if(opponentLegal[i].square.x == 0 && opponentLegal[i].square.y == 7){
      opposite++;
    }else if(opponentLegal[i].square.x == 7 && opponentLegal[i].square.y == 7){
      opposite++;
    }
  }

  maybeCorner = -25.0*opposite;
  double total = (pieceWeight * pieceNum) + (edgeWeight * edge) + (frontierWeight * frontier) + (cornerWeight * corner) + (maybeCornerWeight * maybeCorner) + (mobilityWeight * mobility);

  return total;
}

int Game::alphaBeta(Board board, int depth, int alpha, int beta, bool playerMax){ //minimax DFS with alpha beta pruning
  int a, b;
  a = alpha;
  b = beta;

  if((((float)(clock()-tStart))/CLOCKS_PER_SEC) > (tCutoff * tLimit)){
    noTime = true;
    return heuristic(board);
  }else if(depth == 0){
    return heuristic(board);
  }else{
    depth--;
  }

  vector<Board::boardMove> moveOption = board.discoverMoves(board.currentPlayer);

  if(moveOption.size() == 0){
    if(board.checkEnd(true)){
      Board childBoard = board;
      childBoard.nextPlayer(false);
      return heuristic(childBoard);
    }else{
      Board childBoard = board;
      childBoard.nextPlayer(true);
      return alphaBeta(childBoard, depth, alpha, beta, !playerMax);
    }
  }

  if(playerMax){  //max part
    int v = INT_MIN;

    for(int i = 0; i < moveOption.size(); i++){
      Board childBoard = board;
      childBoard.makeMove(moveOption[i]);
      childBoard.nextPlayer(false);

      int value = alphaBeta(childBoard, depth, a, b, false);
      v = max(v, value);

      if(v >= beta){
        return v;
      }
      int a = max(a,v);
    }

    return v;
  }else{//min part
    int v = INT_MAX;

    for(int i = 0; i < moveOption.size(); i++){
      Board childBoard = board;
      childBoard.makeMove(moveOption[i]);
      childBoard.nextPlayer(false);

      int value = alphaBeta(childBoard, depth, a, b, true);
      v = min(v, value);

      if(v <= a){
        return v;
      }
      b = min(b, v);
    }

    return v;
  }
}

bool Game::humanMove(){ //implementing how a player would choose moves
  int moveChoice = -1;
  char choice;

  vector<Board::boardMove> options = board.discoverMoves(board.currentPlayer);

  if(options.size()){
    board.print(options);
    cout << "List of legal moves:" << endl;

    for(int i = 0; i < options.size(); i++){
      cout << (char)(i+65) << ": (" << (int)options[i].square.y << "," << (int)options[i].square.x << ")" << endl;
    }

    while(!(moveChoice < options.size() && moveChoice >= 0)){
      cout << "Choose a move: ";
      cin >> choice;
      moveChoice = ((int)toupper(choice) - 65);
    }

    board.makeMove(options[moveChoice]);
    return board.nextPlayer(false);
  }else{
    cout << "No legal moves available, passing turn" << endl;
    return board.nextPlayer(true);
  }
}

bool Game::aiMove(){  //ai move generation using iterative deepening method
  int depth, limit, value, moveChoice;
  limit = squares - (board.score[black] + board.score[white]);
  Board::boardMove moveOption, moveBest;

  tStart = clock();
  playerMax = board.currentPlayer;

  vector<Board::boardMove> moveLegal = board.discoverMoves(board.currentPlayer);

  if(moveLegal.size() == 0){
    cout << "No legal moves available for computer, passing turn" << endl;
    return board.nextPlayer(true);
  }

  for(depth = 0; (((float)(clock()-tStart))/CLOCKS_PER_SEC < tLimit/2.0) && depth < limit; depth++){
    int alpha, beta, random;
    alpha = INT_MIN;
    beta = INT_MAX;
    random = 1;
    noTime = false;

    for(int i = 0; i < moveLegal.size(); i++){
      Board childBoard = board;
      childBoard.makeMove(moveLegal[i]);
      childBoard.nextPlayer(false);
      value = alphaBeta(childBoard, depth, alpha, beta, false);

      if(noTime){
        break;
      }

      if(value > alpha){
        moveOption = moveLegal[i];
        moveChoice = i;
        alpha = value;
      }else if(value == alpha){
        if(((rand() % random++) - 1) == 0){
          moveOption = moveLegal[i];
          moveChoice = i;
        }
      }
    }
    moveBest = moveOption;
  }

  board.print(vector<Board::boardMove>(1, moveBest), true);
  cout << "Max Depth Searched: " << depth << " [time taken = " << ((float)(clock()-tStart))/CLOCKS_PER_SEC << " seconds]" << endl;
  cout << "Move chosen by computer: " << (char)(moveChoice+65) << endl;

  board.makeMove(moveBest);

  return board.nextPlayer(false);
}

bool Game::randomMove(){  //random move generation
  vector<Board::boardMove> options = board.discoverMoves(board.currentPlayer);

  if(options.size()){
    Board::boardMove random = options[rand() % options.size()];
    board.print(vector<Board::boardMove>(1,random), true);
    board.makeMove(random);
    return board.nextPlayer(false);
  }else{
    cout << "No legal moves available for computer, passing turn" << endl;
    return board.nextPlayer(true);
  }
}
