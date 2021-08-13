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

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_)  {
    
}

FallingLetters::~FallingLetters() {
    world->blur = false;
  //  world->deleteParticleEffect(letters);
 //   letters->~ComplexParticles();
    if (target != NULL) {
        target->setState(STATE_NORMAL);
    }
    world->deleteForce(force);
}

void FallingLetters::call(Game* game) {
 //   letters->init(0.1, actor->getPos(), 3, 4, 3, 100, 2, 1000);
   // flowers->init(0.028, actor->getPos(), 4, 3, 4, 350, 1);
  //  test->init(actor->getPos(), 3, 4, 3, 150, 2);
   // world->insertParticleEffect(letters);
   // world->blur= true;
    if (target != NULL) { 
    target->setState(STATE_PARALYZED);
        force = new Force();
        force->init(glm::vec3(0,0,0), 1.25f);
        force->setActor(target.get());
        force->configureVortexForce(0.96, -0.1f, FORCE_LINEAR);
        world->insertForce(force); 
                 
        up = new Force();
        up->init(glm::vec3(0,0,0), 0.5f);
        up->setActor(target.get());
        up->configureStraightForce(glm::vec3(0,1,0), -0.1f, FORCE_QUADRATIC);
         world->insertForce(up);
    } 
  //  world->insertParticleEffect(flowers);
   // world->insertParticleEffect(test);
}

void FallingLetters::tick() { 
    duration -= glfwGetTime();
    if (duration < 0) {
        on = false;
    }
}

