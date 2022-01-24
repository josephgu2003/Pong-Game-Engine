//
//  CharacterComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#include "CharacterComponent.hpp"
#include <iostream>
#include "Actor.hpp"

#include "aiPonder.hpp"
#include "aiSingFriend.hpp"

CharacterComponent::CharacterComponent(Actor& actor) : NameComponent(actor) {
    updatePriority = 1; 
    nextOpenTrait = 0;
    for (int i = 0; i < MAX_PTRAITS; i++) {
        traits[i] = PT_EMPTY;
    }
    currentAction = std::make_shared<aiPonder>(10.0f);
}

void CharacterComponent::tick() { 
    if (currentAction.get()) {
        currentAction->tick(static_cast<Actor*>(actor));
        if (currentAction->isDone()) {
            currentAction.reset();
        }
    }
}
 

void CharacterComponent::newRelationship(const std::shared_ptr<Relationship>& rs_) {
    std::pair<std::string, std::shared_ptr<Relationship>> rs;
    if (rs_->getName(RS_ONE) == name) {
        rs.first = rs_->getName(RS_TWO); 
    } else if (rs_->getName(RS_TWO) == name) {
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
        auto it = relationships.find(actor->getComponent<CharacterComponent>()->getName());
        if (it != relationships.end()) {
            intensity = it->second->getIntensity();
            return true;
        }
    
    return false;
}
bool CharacterComponent::hasRelationshipWith(Actor* actor, RelationShipType rst, Relationship*& r) {
        auto it = relationships.find(actor->getComponent<CharacterComponent>()->getName());
        if (it != relationships.end()) {
            r = it->second.get(); 
            return true;
        }
    
    return false;
}

RelationShipType CharacterComponent::getRelationshipTypeWith(Actor* actor) {
    auto c = actor->getComponent<CharacterComponent>();
    if (!c) {
        return RS_EMPTY;
    }
    auto it = relationships.find(c->getName());
    if (it != relationships.end()) {
        return it->second->getType();
    }
    return RS_EMPTY;
}

void CharacterComponent::newAction(aiActionEnum action, Actor* target) {
    switch (action) {
        case ACT_PONDER:
            currentAction = std::make_shared<aiPonder>(10.0f);
            break;
        case ACT_DANCE:
            currentAction = std::make_shared<aiDance>(10.0f);
            break;
        case ACT_WATCH_SCENERY:
            break; 
        case ACT_SING_FRIEND:
            if (target != nullptr)
            currentAction = std::make_shared<aiSingFriend>(10.0f, target);
            break;
        case ACT_GIVE_GIFT:
            break;
        case ACT_DIALOGUE:
            break;
    }
}
 
bool CharacterComponent::hasCurrentAction() {
    if (currentAction.get() != NULL) {
        return true;
    } 
        return false; 
} 

std::vector<QA> CharacterComponent::getQAWith(Actor* target) {
    std::string targetname = target->getComponent<CharacterComponent>()->getName();
    auto it = relationships.find(targetname);
    if (it != relationships.end()) {
        std::vector<QA> vec = (*it).second->getQuestionsFor(targetname);
        return vec;
    }
    std::vector<QA> v = {QA("Who are you?", "Not telling u")};
    return v; 
} 

void CharacterComponent::newAction(const std::shared_ptr<aiAction>& a) {
    currentAction = a;
}
