//
//  CombatComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "CombatComponent.hpp"
#include "Actor.hpp"

void CombatComponent::tick(Actor& actor, World& world) {
    switch (actor.getState()) {
        case STATE_NORMAL:
            // world->getfieldeffect->affect(actor)
            break;
        case STATE_PARALYZED:
            
            break;
    }
}
