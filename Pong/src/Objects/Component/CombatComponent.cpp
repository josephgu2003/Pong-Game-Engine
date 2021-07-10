//
//  CombatComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "CombatComponent.hpp"
#include "Actor.hpp"
#include "World.hpp"
CombatComponent::CombatComponent() {
    
}

void CombatComponent::tick(Actor& actor, World& world) {
    world.informActorProximity(actor, 5);
}
