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

class Game;

class Ability {
protected:
    World* world;
    Actor* actor;
    Actor* target = NULL;
    float duration;
    Game* game;
    int step;
public:
    bool on;
    Ability();
    Ability(World* world_, Actor* actor_, float duration_);
    virtual void setTarget(Actor* actor_);
    virtual ~Ability();
    virtual void call(Game* game) = 0;
    virtual void tick() = 0;
    virtual void dispel();
    int getStep();
};

#endif /* Ability_hpp */
