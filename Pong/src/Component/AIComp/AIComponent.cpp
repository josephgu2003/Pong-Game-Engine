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
 
#define DICE_MAX 100

typedef std::vector<std::shared_ptr<Actor>> ActorList;

AIComponent::AIComponent(Actor& actor) : Component(actor) {
    type = AI;
    dice = Dice(0, DICE_MAX);
    currentAction.reset();
    currentAction = std::make_unique<aiDance>(10.0f);
    for (int i = 0; i < NUM_ACTIONS; i++) {
        std::pair<aiActionEnum, float> prob;
        prob.first = aiActionTable[i].action;
        prob.second = aiActionTable[i].baseProb;
        probMap.insert(prob);
    }
}

void AIComponent::tick() {
    if (currentAction.get() != nullptr) { // if we have a current action
        if (!currentAction->isDone()) {
            currentAction->tick(actor);
        } else {
            currentAction.reset();
        }
    } else { //else give ourselves a new action
        const ActorList al = actor->getWorld().getNearActorsWith(actor, CHAR);
        for (int i = 0; i < al.size(); i++) {
            auto x =al.at(i).get();
            calcProbabilities(actor,x);
            calcNextAction(actor, x);
        }
        calcProbabilities(actor, nullptr);
        calcNextAction(actor, nullptr);
    }
}
 

void AIComponent::calcNextAction(Actor* actor, Actor* target) {
    int c = 3;
    while (c != 0) {
        for (auto i = probMap.begin(); i != probMap.end(); i++) {
            int roll = dice.roll();
            if (roll < DICE_MAX*i->second) {
                newAction(i->first, target);
                return;
            }; 
        }
        c -= 1;
    }
}
 
void AIComponent::calcProbabilities(Actor* actor, Actor* target) { // NEED HEAVY OPTIMIZATION
    // need to know the environment
    
    for (auto i = probMap.begin(); i != probMap.end(); i++) {// iterate over prob map
        
        const aiActionAffinity* entry;
        for (int j = 0; j < NUM_ACTIONS; j++) { // over ai actiontable to find the entry
            if (i->first == aiActionTable[j].action) {
                entry = &aiActionTable[j];
                i->second = aiActionTable[j].baseProb;
                break;
            }
        }
        
        if (!actor->hasComponent(CHAR)) { //if actor has no char comp, skip rest
            continue;
        }
        auto c =  actor->getComponent<CharacterComponent>();
        
        
        for (int count = 0; count < MAX_PTRAITS; count++) { //for ptraits
            float bonus = 0;
            if (entry->affinities[count] != PT_EMPTY && c->hasTrait(entry->affinities[count])) bonus = 0.15;
            i->second += bonus;
        }
        
        if (target == nullptr) continue;
        
        for (int z = 0; z < MAX_RS; z++) { // iterate over num of rs
            float bonus = 0;
         
                if (entry->rsFactors[z] == RS_EMPTY) continue;
                
                float intensity;
                if (c->hasRelationshipWith(target, entry->rsFactors[z], intensity)) {
                    bonus = std::max(bonus, intensity/10.0f); // PROBLEM:: later on will target  a random relationship holder, not the most intense guy 
                    // fix, need probMap for each char, roll for each
                }
        
            i->second += bonus;
        }


    }
}

void AIComponent::newAction(aiActionEnum action, Actor* target) {
    switch (action) {
        case ACT_PONDER:
            currentAction = std::make_unique<aiPonder>(10.0f);
            break;
        case ACT_DANCE:
            currentAction = std::make_unique<aiDance>(10.0f);
            break;
        case ACT_WATCH_SCENERY: 
            break; 
        case ACT_SING_FRIEND: 
            if (target != nullptr)
            currentAction = std::make_unique<aiSingFriend>(10.0f, target);
            break;
        case ACT_GIVE_GIFT: 
            break;
    }
}
 
