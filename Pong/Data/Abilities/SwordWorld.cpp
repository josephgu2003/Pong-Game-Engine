//
//  SwordWorld.cpp
//  Pong
//
//  Created by Joseph Gu on 12/17/21.
//

#include "SwordWorld.hpp"
#include "ParticleFactory.hpp"
#include "Actor.hpp"

#include "World.hpp"

void SwordWorld::call() {
    ParticleFactory pf;
    std::shared_ptr<ParticleSystem> swords = pf.makeParticles(PE_MOONBELL_SWORDS, actor->getPos()+glm::vec3(0,4,0));
    swordsRef = swords;
    world->insert<ParticleSystem>(swords);
}

void SwordWorld::tick() {
    duration -= (float)glfwGetTime();
}

SwordWorld::~SwordWorld() {
    if (auto x = swordsRef.lock()) {
        world->deleteX<ParticleSystem>(x.get());
    }
} 
