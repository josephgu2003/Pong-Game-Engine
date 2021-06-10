//
//  GameBoard.hpp
//  ttt
//
//  Created by Joseph Gu on 4/21/21.
//

#ifndef GameGrid_hpp
#define GameGrid_hpp

#include <stdio.h>
#include "GameStatus.hpp"

class GameGrid {
    char grid [3][3] = {{' ', ' ', ' '},{' ', ' ', ' '},{' ', ' ', ' '}};
public:
    char getElementAt (int a, int b);
    void updateGrid (int r, char c, char playerinput);
    char(*getReferencetoArray())[3];
};

#endif /* GameGrid_hpp */
