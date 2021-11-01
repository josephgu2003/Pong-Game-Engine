//
//  CombatComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "CombatComponent.hpp"
#include "Actor.hpp"
#include "World.hpp"
CombatComponent::CombatComponent(Actor& actor) : ActorComp(actor) {
    Component::type = COMBAT;
    biggestTarget = std::make_shared<Actor>(); 
}

void CombatComponent::tick() {
    actor->getWorld().informActorProximity(*actor, 5);
}
void CombatComponent::clearAffecting() {
    affecting.reset();
}
void CombatComponent::newAbility(const std::shared_ptr<Ability>& ab) {
    abilityQ.push_back(std::move(ab));
}
void CombatComponent::affect(std::shared_ptr<Ability> ab) {
    affecting = ab;
}
void CombatComponent::setBigTarget(const std::shared_ptr<Actor>& ac) {
    biggestTarget.reset();
    biggestTarget = ac;
}
std::shared_ptr<Actor>& CombatComponent::getBigTarget() {
    return biggestTarget;
}
bool CombatComponent::hasTarget() {
    if (biggestTarget.get() != NULL) {
        return true;
    }
    return false; 
}
bool CombatComponent::QHasAbilities() {
    if (abilityQ.size() > 0) {
        return true;
    }
    return false;
}
  

std::shared_ptr<Ability> CombatComponent::getAffecting() {
    return affecting;
}

std::vector<std::shared_ptr<Ability>>& CombatComponent::getAbilityQ() {
    return abilityQ;
}
