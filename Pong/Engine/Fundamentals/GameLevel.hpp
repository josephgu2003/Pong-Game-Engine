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

class Renderer;

class GameLevel {
private:
    std::vector<std::shared_ptr<World>> worlds;
    std::weak_ptr<World> activeWorld; 
    std::string nextLevel = "";
    std::string name;
public:
    GameLevel(Renderer* r, int numWorlds, std::string name);
    ~GameLevel();
    std::string getName();
    std::string tick();
    World* getActiveWorld();
    int getNumWorlds();
    World& getWorld(int i);
    void swapWorld(int i);
    void changeLevel(std::string newLevel);
};

#endif /* GameState_hpp */
