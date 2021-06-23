//
//  ProximityComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#include "BehaviorComponent.hpp"
#include "World.hpp"

void BehaviorComponent::tick(Actor& actor, World& world) {
    world.informProximityComponent(actor, *this);
    if (somethingNear) {
        trigger(actor,world);
        somethingNear= false;
    }
}

