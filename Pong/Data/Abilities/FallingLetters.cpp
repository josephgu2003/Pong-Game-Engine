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
#include "LifeComponent.hpp"

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_)  {
    
}

FallingLetters::~FallingLetters() {
    world->blur = false;
    world->deleteParticleEffect(letters);
    if (target != NULL) {
        target->setState(STATE_IDLE);
    }
    world->deleteForce(force);
    world->deleteForce(up);
    delete force;
    delete up;
}

void FallingLetters::call() {
    letters = new RunicLetters(); 
    letters->init(0.2, actor->getPos(), glm::vec3(10.0,10.0,10.0), 50, 20, 10.0, 0.999);
 
    world->insertParticleEffect(letters); 
    world->blur= true;
    
    if (target) {
        target->setState(STATE_PARALYZED);
        
        force = new Force();
        force->init(glm::vec3(0,0,0), 1.25f); 
        force->setActor(target); 
        force->configureVortexForce(0.96, -0.1f, FORCE_LINEAR);
        world->insertForce(force); 
                 
        up = new Force(); 
        up->init(glm::vec3(0,0,0), 0.5f);
        up->setActor(target);
        up->configureStraightForce(glm::vec3(0,1,0), -0.1f, FORCE_QUADRATIC);
         world->insertForce(up);
    }
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

 
 
