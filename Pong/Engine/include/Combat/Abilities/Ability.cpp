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

Ability::Ability(World* world_, Actor* actor_, float duration_, std::weak_ptr<Actor>& target_) {
    world=world_;
    actor=actor_;
    duration = duration_;
    on = true;
    step = 0;
    target = target_;
}

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
    notifyTarget();
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

void Ability::notifyTarget() {
    if (auto a = target.lock()) {
        if (auto cc = a->getComponent<CombatComponent>()) {
            cc->affect(shared_from_this());
        }
    }
}
