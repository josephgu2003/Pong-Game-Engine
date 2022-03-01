//
//  GameState.cpp
//  Pong
//
//  Created by Joseph Gu on 11/26/21.
// 

#include "GameLevel.hpp"

GameLevel::~GameLevel() {

}

GameLevel::GameLevel(Renderer* r, int numWorlds, std::string name_) {
    name = name_;
    for (int i = 0; i < numWorlds; i++) {
        worlds.push_back(std::make_shared<World>(r));
    }
    swapWorld(0); 
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

void GameLevel::swapWorld(int i) {
    activeWorld = worlds.at(i);
    activeWorld.lock()->activate();
}

std::string GameLevel::getName() {
    return name;
}

int GameLevel::getNumWorlds() {
    return worlds.size();
}


