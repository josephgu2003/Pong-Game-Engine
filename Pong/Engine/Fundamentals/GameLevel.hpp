//
//  GameState.hpp
//  Pong
//
//  Created by Joseph Gu on 11/26/21.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include "World.hpp"
class GameLevel {
private:
    std::vector<World> worlds;
public:
    GameLevel(int numWorlds);
};

#endif /* GameState_hpp */
