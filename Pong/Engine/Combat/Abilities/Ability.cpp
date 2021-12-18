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
    target = std::weak_ptr<Actor>();
};

Ability::~Ability() {
    
}

void Ability::setTarget(const std::weak_ptr<Actor>& actor_) {
    target = actor_;
}

void Ability::dispel() {
    on = false;
    if (auto x = target.lock()) {
        x->getComponent<CombatComponent>()->clearAffecting(); 
    }
//    static_pointer_cast<CombatComponent>(target->getComp(COMBAT))->clearAffecting();
}

int Ability::getStep() {
    return step;
}
