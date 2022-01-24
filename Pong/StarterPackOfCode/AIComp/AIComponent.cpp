//
//  AIComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#include "AIComponent.hpp"
#include "Actor.hpp"
#include "aiPonder.hpp"
#include "World.hpp"
#include <memory>
#include "aiSingFriend.hpp"
#include <algorithm>
#include <iterator> 
 
#define DICE_MAX 100
#define getCharComp actor->getComponent<CharacterComponent>()

typedef std::vector<std::shared_ptr<Actor>> ActorList;

AIComponent::AIComponent(Actor& actor) : Component(actor) {
    updatePriority = 2;
    dice = Dice(0, DICE_MAX);
    entriesInTable = 0;
    actionTable = new aiActionAffinity();
    resetTable(RS_EMPTY);
}

void AIComponent::tick() {
    if (!getCharComp->hasCurrentAction()) { // if we have not a current action
        const ActorList al = static_cast<Actor*>(actor)->getWorld().getNearActorsWith<CharacterComponent>(static_cast<Actor*>(actor));
        for (auto i = al.begin(); i != al.end(); i++) {
            auto x = (*i).get();
            resetTable(getCharComp->getRelationshipTypeWith(x));
            calcProbabilities(static_cast<Actor*>(actor), x); 
            calcNextAction(static_cast<Actor*>(actor), x);
            if (getCharComp->hasCurrentAction()) return;
        }
    }
}
  

void AIComponent::calcNextAction(Actor* actor, Actor* target) {
    int c = 3;
    while (c != 0) {
        for (int i = 0; i < entriesInTable; i++) {
            int roll = dice.roll();
            if (roll < DICE_MAX*actionTable[i].baseProb) {
                getCharComp->newAction(actionTable[i].action, target);
                return;
            };
        }
        c -= 1;
    } 
}
 
void AIComponent::calcProbabilities(Actor* actor, Actor* target) {
    if (!actor->hasComponent<CharacterComponent>()) { //if actor has no char comp, skip rest
        return;
    }
    
    for (int i = 0; i < entriesInTable; i++) {// iterate over prob map

        auto c = getCharComp;

        for (int count = 0; count < MAX_PTRAITS; count++) { //for ptraits
            float bonus = 0;
            if (actionTable[i].affinities[count] != PT_EMPTY && c->hasTrait(actionTable[i].affinities[count])) bonus = 0.15;
            actionTable[i].baseProb += bonus;
        }
        
        if (target == nullptr) continue;
    }
}


 
void AIComponent::resetTable(RelationShipType rst) {
    delete[] actionTable;
    switch (rst) {
        case RS_EMPTY: {
            actionTable = new aiActionAffinity[NUM_PASSIVE_ACTS];
            entriesInTable = NUM_PASSIVE_ACTS;
         //   memcpy(actionTable, aiPassiveActions, sizeof(aiPassiveActions));
            std::copy(aiPassiveActions, aiPassiveActions+NUM_PASSIVE_ACTS, actionTable);
            return;
        }
            
        case RS_FRIEND: { 
            actionTable = new aiActionAffinity[NUM_FRIEND_ACTS];
            entriesInTable = NUM_FRIEND_ACTS; 
            std::copy(aiFriendActions, aiFriendActions+NUM_FRIEND_ACTS, actionTable);
         //   memcpy(actionTable, aiFriendActions, sizeof(aiFriendActions));
            return;
        }
    }
} 
 
 
 
