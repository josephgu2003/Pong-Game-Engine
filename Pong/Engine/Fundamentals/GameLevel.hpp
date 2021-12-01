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
#include "uiLayout.hpp"
#include "InputHandler.hpp"
#include <map>

class Renderer;

class GameLevel {
private:
    std::vector<std::shared_ptr<World>> worlds;
    std::weak_ptr<World> activeWorld;
    std::string nextLevel = "";
public:
    GameLevel(Renderer* r, int numWorlds);
    std::string tick();
    World* getActiveWorld();
    World& getWorld(int i);
    void changeLevel(std::string newLevel);
};

#endif /* GameState_hpp */
