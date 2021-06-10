//
//  GameAI.hpp
//  ttt
//
//  Created by Joseph Gu on 4/22/21.
//

#ifndef GameAI_hpp
#define GameAI_hpp

#include <stdio.h>
#include "GameStatus.hpp"
#include <algorithm>

class GameAI {
public:
    GameStatus* checkGameOver (char grid[3][3]);
    int minimax (char grid[3][3], bool isAI, int step);
};

#endif /* GameAI_hpp */
