/*Donghyun Park
Prof. Sable
Artificial Intelligence: Othello
11/5/17*/

#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char **argv){
  Game othello;
  int type;

  cout << "Donghyun Park's Othello AI Project" << endl
       << "Choose how to play: " << endl
       << "1: Black Pieces (first turn)" << endl
       << "2: White Pieces (second turn)" << endl
       << "3: AI vs AI" << endl;
  while (1) {
    cout << "Choosing: ";
    cin >> type;

    if((type == 1) || (type == 2) || (type == 3)){
      othello.setGame(type);
      break;
    }else{
      cout << "Please choose between 1 and 3: " << endl;
    }
  }
  othello.playGame();
  return 0;
}
