//
//  GameBoard.cpp
//  ttt
//
//  Created by Joseph Gu on 4/21/21.
//

#include "GameGrid.hpp"

char GameGrid::getElementAt (int a, int b) {
    return grid [a][b];
}

void GameGrid::updateGrid (int r, char c, char playerinput) {
    if (c == 'a') {
        grid [r-1][0] = playerinput;
    } else if (c == 'b') {
        grid [r-1][1] = playerinput;
    } else if (c == 'c') {
        grid [r-1][2] = playerinput;
    }
}

char (*GameGrid::getReferencetoArray())[3]{
    return grid;
}


