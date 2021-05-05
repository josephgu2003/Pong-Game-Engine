//
//  World.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef World_hpp
#define World_hpp

#include <vector>
#include <iostream>
#include <stdio.h>
#include "Actor.hpp"
#include "Ball.hpp"
#include "PlayerHero.hpp"

class World {
    std::vector <Actor*> allActorPtrs;
    int verticesCount;
public:
    World();
    ~World();
    float* getVertices();
    int getVerticesCount();
    Actor* getNthActor(int n);
};
#endif /* World_hpp */
