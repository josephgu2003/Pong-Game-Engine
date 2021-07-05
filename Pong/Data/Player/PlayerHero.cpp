//
//  PlayerHero.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "World.hpp"
#include "PlayerHero.hpp"
#include "Particle.hpp"
#include "PhysicsComponent.hpp"

PlayerHero::PlayerHero() : Actor::Actor() {
    PhysicsComponent* pC = new PhysicsComponent(true);
    components.push_back(pC);
}

PlayerHero::~PlayerHero() {
    
}


