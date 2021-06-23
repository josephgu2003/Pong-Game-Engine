//
//  FallingLetters.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "FallingLetters.hpp"
#include "Particle.hpp"

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_) {
    
}

void FallingLetters::call(Game* game) {
    letters->init(0.1, actor->getPos(), 3, 4, 3, 100, 2);
   // flowers->init(0.028, actor->getPos(), 4, 3, 4, 350, 1);
  //  test->init(actor->getPos(), 3, 4, 3, 150, 2);
    world->insertParticleEffect(letters);
    world->blur= true;
    if (target != NULL) {
    target->state = STATE_PARALYZED;
    }
  //  world->insertParticleEffect(flowers);
   // world->insertParticleEffect(test);
}

void FallingLetters::tick() {
    duration -= glfwGetTime();
    if (duration < 0) {
        on = false;
        world->blur = false;
        world->deleteParticleEffect(letters);
        letters->~ComplexParticles();
        if (target != NULL) {
        target->state = STATE_NORMAL;
        }
    }
}

