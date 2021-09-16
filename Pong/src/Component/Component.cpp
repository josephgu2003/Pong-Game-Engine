//
//  Component.cpp
//  Pong
//
//  Created by Joseph Gu on 5/29/21.
//

#include "Component.hpp"
#include "Actor.hpp"
#include "World.hpp"

Component::Component() {
    
}

Component::Component(Actor& actor_) {
    actor = &actor_;
}

CompType Component::getType() {
    return type;
}
