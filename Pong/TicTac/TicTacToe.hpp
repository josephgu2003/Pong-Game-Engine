//
//  TicTacToe.hpp
//  Pong
//
//  Created by Joseph Gu on 6/1/21.
//

#ifndef TicTacToe_hpp
#define TicTacToe_hpp

#include <stdio.h>
#include <string>

//  main.cpp
//  ttt
//
//  Created by Joseph Gu on 4/16/21.
//
#include "GameGrid.hpp"
using namespace std;

class TicTacToe {
public:
    void printgrid(GameGrid& gameGrid);
    string fetchPlayerMove();
    int init();
};
#endif /* TicTacToe_hpp */
