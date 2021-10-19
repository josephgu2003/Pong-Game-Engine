//
//  CharacterComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#include "CharacterComponent.hpp"
#include <iostream>
#include "Actor.hpp"

CharacterComponent::CharacterComponent(Actor& actor) : NameComponent(actor) {
    Component::type = CHAR;
    nextOpenTrait = 0;
    for (int i = 0; i < MAX_PTRAITS; i++) {
        traits[i] = PT_EMPTY;
    }
}

void CharacterComponent::tick() { 
    
}
 

void CharacterComponent::newRelationship(const std::shared_ptr<Relationship>& rs_) {
    std::pair<std::string, std::weak_ptr<Relationship>> rs;
    if (rs_->getName(RS_ONE) == idName) {
        rs.first = rs_->getName(RS_TWO);
    } else if (rs_->getName(RS_TWO) == idName) {
        rs.first = rs_->getName(RS_ONE);
    } else {
        std::cout << "Error: Relationship assigned to wrong target" << "\n";
    }
    rs.second = rs_;
    relationships.insert(std::move(rs));
}
 

void CharacterComponent::newTrait(PersonalityTrait pt) {
    traits[nextOpenTrait] = pt;
    
    if (nextOpenTrait < MAX_PTRAITS-1) {
        nextOpenTrait++;
    }
}

bool CharacterComponent::hasTrait(PersonalityTrait pt) {
    for (int i = 0; i < MAX_PTRAITS; i++) {
        if (traits[i] == pt) return true;
    }
    return false;
}

bool CharacterComponent::hasRelationshipWith(Actor* actor, RelationShipType rst, float& intensity) {
    for (int i = 0; i < relationships.size(); i++) {
        auto it = relationships.find(actor->getComponent<CharacterComponent>()->getIdName());
        if (it != relationships.end()) {
            intensity = it->second->getIntensity();
            return true;
        }
    }
    return false;
}
