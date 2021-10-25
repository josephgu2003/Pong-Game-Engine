//
//  FallingLetters.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "FallingLetters.hpp"
#include "Particle.hpp"
#include <memory>
#include "Force.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "LifeComponent.hpp"

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_)  {
    
}

FallingLetters::~FallingLetters() {
    world->blur = false;
    world->deleteParticleEffect(letters);
    if (target != NULL) {
        target->setState(STATE_IDLE);
    }

}
 
void FallingLetters::call() {
    letters = new ParticleSystem();
    letters->init(PE_RUNICLETTERS, actor->getPos()+glm::vec3(0,3,0));
  
    world->insertParticleEffect(letters); 
    world->blur= true;

}

void FallingLetters::tick() { 
    duration -= glfwGetTime();
    if (duration < 0) {
        on = false;
    }
    if (target->getComponent<LifeComponent>()) {
        target->getComponent<LifeComponent>()->incStatValue(-0.0005, STAT_LIFE);
    }
}

 
 
