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

CompType Component::getType() {
    return type;
}
