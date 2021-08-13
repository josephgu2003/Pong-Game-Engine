//
//  Ability.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "Ability.hpp"
#include "Game.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "CombatComponent.hpp"

Ability::Ability(){}

Ability::Ability(World* world_, Actor* actor_, float duration_) {
    world=world_;
    actor=actor_;
    duration = duration_;
    on = true;
    step = 0;
};

Ability::~Ability() {
    
}

void Ability::setTarget(const std::shared_ptr<Actor>& actor_) {
    target = actor_;
}

void Ability::dispel() {
    on = false;
    static_pointer_cast<CombatComponent>(target->getComp(COMBAT))->clearAffecting();
}

int Ability::getStep() {
    return step;
}
