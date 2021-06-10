//
//  GameStatus.cpp
//  ttt
//
//  Created by Joseph Gu on 4/21/21.
//

#include "GameStatus.hpp"
GameStatus::GameStatus(bool gameOver_, int nodeValue_){
    gameOver = gameOver_;
    nodeValue = nodeValue_;
};
    int GameStatus::getNodeValue() {return nodeValue;}
    int GameStatus::getGameOver() {return gameOver;}
