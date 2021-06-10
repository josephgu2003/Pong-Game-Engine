//
//  Ability.hpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#ifndef Ability_hpp
#define Ability_hpp

#include <stdio.h>
#include "World.hpp"
#include "Actor.hpp"

class Ability {
protected:
    World* world;
    Actor* actor;
    float duration;
public:
    bool on;
    Ability();
    Ability(World* world_, Actor* actor_, float duration_);
    virtual void call() = 0;
    virtual void tick() = 0;
};

#endif /* Ability_hpp */
