//
//  World.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "World.hpp"


World::World() {
    Ball* ball = new Ball;
    
    ball->init();
    allActorPtrs.push_back(ball);
    
    PlayerHero* playerHero = new PlayerHero;
    
    playerHero->init();
    allActorPtrs.push_back(playerHero);
}

World::~World() {
    
}

float* World::getVertices() {
    std::vector<float> worldVertices;
    for (std::vector<Actor*>::iterator it = allActorPtrs.begin() ; it != allActorPtrs.end(); it++) {
     //   std::vector<float> temporaryVector //((*it)->getVertices(),(*it)->getVertices()+(*it)->getVerticesCount());
        worldVertices.insert(worldVertices.end(), (*it)->getVertices(), (*it)->getVertices()+(*it)->getVerticesCount());
    }
    verticesCount = worldVertices.size();
    std::cout << worldVertices.size() << "\n";
    return &worldVertices[0];
}

int World::getVerticesCount() {
    return verticesCount;
}

Actor* World::getNthActor(int n) {
    return allActorPtrs[n];
}
