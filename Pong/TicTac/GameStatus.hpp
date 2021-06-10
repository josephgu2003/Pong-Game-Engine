//
//  GameStatus.hpp
//  ttt
//
//  Created by Joseph Gu on 4/21/21.
//

#ifndef GameStatus_hpp
#define GameStatus_hpp

#include <stdio.h>

class GameStatus {
    bool gameOver;
    int nodeValue;
  public:
    GameStatus(bool gameOver_, int nodeValue_);
    void setGameOver (bool gameOver_);
    void setGameOver (int nodeValue_);
    int getNodeValue();
    int getGameOver();
};

#endif /* GameStatus_hpp */
