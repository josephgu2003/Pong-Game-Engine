//
//  Ability.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "Ability.hpp"


Ability::Ability(){}

Ability::Ability(World* world_, Actor* actor_, float duration_) {
    world=world_;
    actor=actor_;
    duration = duration_;
    on = true;
};

Ability::~Ability() {
    
}

void Ability::setTarget(Actor* actor_) {
    target = actor_;
}

void Ability::dispel() {
    on = false;
    target->affecting = NULL;
}
