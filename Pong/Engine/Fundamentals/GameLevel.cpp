//
//  GameState.cpp
//  Pong
//
//  Created by Joseph Gu on 11/26/21.
//

#include "GameLevel.hpp"
#include "Renderer.hpp"

GameLevel::GameLevel(Renderer* r, int numWorlds) {
    for (int i = 0; i < numWorlds; i++) {
        worlds.push_back(std::make_shared<World>(r));
    }  
    activeWorld = worlds.at(0);
    r->setWorld(worlds.at(0).get()); 
} 

std::string GameLevel::tick() {  
    if (auto x = activeWorld.lock()) {
        x->tick();  
    }
    return nextLevel;
}

World* GameLevel::getActiveWorld() {
    if (auto x = activeWorld.lock()) {
        return x.get();
    }
    return nullptr; 
}

World& GameLevel::getWorld(int i) {
    return *(worlds.at(i).get()); 
}

void GameLevel::changeLevel(std::string newLevel) {
    nextLevel = newLevel;
}
