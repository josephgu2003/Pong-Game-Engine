//
//  PPhysicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/22/21.
//

#include "PPhysicsComponent.hpp"
#include "Particle.hpp" 

PPhysicsComponent::PPhysicsComponent(ParticleSystem& pe, float gravity_, float friction_)  : Component(pe){
    type = ANIM;
    particles = pe.getParticles();
    velocities = new glm::vec3[pe.getNumParticles()];
    numParticles = static_cast<ParticleSystem*>(actor)->getNumParticles();
    gravity = gravity_;
    friction = friction_; 
    for (int i = 0; i < numParticles; i++) {
        velocities[i] = glm::vec3(0);
    }
}

void PPhysicsComponent::tick() {
    for (int i = 0; i < numParticles; i++) {
        velocities[i] += glm::vec3(0, -gravity, 0);
        velocities[i] *= friction;
        particles[i].posVec += velocities[i];
    }
}

glm::vec3* PPhysicsComponent::getVelocities() {
    return velocities; 
}
