//
//  CombatComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "CombatComponent.hpp"
#include "Actor.hpp"
#include "World.hpp"
CombatComponent::CombatComponent(Actor& actor) : Component(actor) {
    Component::type = COMBAT;
    biggestTarget = std::weak_ptr<Actor>();
    affecting = std::weak_ptr<Ability>();
}

void CombatComponent::tick() {
    static_cast<Actor*>(actor)->getWorld().informActorProximity(*static_cast<Actor*>(actor), 5);
} 
void CombatComponent::clearAffecting() {
    affecting.reset();
}
void CombatComponent::newAbility(const std::shared_ptr<Ability>& ab) {
    abilityQ.push_back(std::move(ab));
}
void CombatComponent::affect(const std::shared_ptr<Ability>& ab) {
    affecting = ab;
}
void CombatComponent::setBigTarget(const std::shared_ptr<Actor>& ac) {
    if (ac->getComponent<CombatComponent>()) {
        biggestTarget.reset();
        biggestTarget = ac;
    } 
}
std::weak_ptr<Actor>& CombatComponent::getBigTarget() {
    return biggestTarget;
}
bool CombatComponent::hasTarget() {
    if (biggestTarget.lock().get() != NULL) {
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
  

std::weak_ptr<Ability> CombatComponent::getAffecting() {
    return affecting;
}

std::vector<std::shared_ptr<Ability>>& CombatComponent::getAbilityQ() {
    return abilityQ;
}
