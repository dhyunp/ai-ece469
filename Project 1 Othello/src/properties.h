#ifndef _PROPERTIES_H
#define _PROPERTIES_H

#define squares 64
#define black 1 //black starts the game
#define white 2

#define modes 4
#define boardDirection 2

#define tCutoff 0.9

#define max(a,b) ((a>b) ? a:b)
#define min(a,b) ((a<b) ? a:b)

//referenced from https://gist.github.com/vratiu/9780109

#define cyan "\033[0;36m" //black player's color
#define red "\033[0;31m" //white player's color
#define green "\033[0;32m" //legal moves options color
#define yellow "\033[0;33m" //move chosen by AI color
#define reset "\033[0m" //reset color

#endif //_PROPERTIES_H
