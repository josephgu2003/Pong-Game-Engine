//
//  PlayerHero.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "World.hpp"
#include "PlayerHero.hpp"
#include "Particle.hpp"

PlayerHero::PlayerHero() : Actor::Actor() {
    light = {true,0};
}

PlayerHero::~PlayerHero() {
    
}

void PlayerHero::makeParticle() {
}

void PlayerHero::sheathSword() {
    swordSheath = true;
}
void PlayerHero::unsheathSword() {
    swordSheath = false;
}

void PlayerHero::fix() {
    
}
