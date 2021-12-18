//
//  Ability.hpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#ifndef Ability_hpp
#define Ability_hpp

#include <stdio.h>
#include <memory>

class Game;

class World;

class Actor;
 
class Ability {
protected: 
    World* world = NULL;
    Actor* actor = NULL;
    std::weak_ptr<Actor> target;
    float duration;
    int step;
public:
    bool on;
    Ability(); 
    Ability(World* world_, Actor* actor_, float duration_);
    virtual void setTarget(const std::weak_ptr<Actor>& actor_);
    virtual ~Ability();
    virtual void call() = 0;
    virtual void tick() = 0;
    virtual void dispel();
    int getStep();
};

#endif /* Ability_hpp */
