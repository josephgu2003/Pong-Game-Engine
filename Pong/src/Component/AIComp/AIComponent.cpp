//
//  AIComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#include "AIComponent.hpp"
#include "Actor.hpp"

AIComponent::AIComponent(Actor& actor) : Component(actor) {
    type = AI;
    dice = Dice(0, 100);
    currentAction.reset();
}

void AIComponent::tick() {
    if (currentAction.get() != nullptr) { // if we have a current action
        currentAction->tick(actor);
    }
}
 

void AIComponent::calcNextAction(Actor& actor) {
    // roll dice with action map
}
