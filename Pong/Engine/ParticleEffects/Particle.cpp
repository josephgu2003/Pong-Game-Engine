//
//  Particle.cpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#include "Particle.hpp"
#include "World.hpp"
 

ParticleSystem::ParticleSystem(int numParticles_, float duration_) {
    particles = new Particle[numParticles_];
    numParticles = numParticles_;  
    for (int i = 0; i < numParticles; i++)
    {
        particles[i] = Particle();
    }
    duration = duration_; 
    watch.resetTime();
}

ParticleSystem::~ParticleSystem() {
    delete[] particles;
}
  
void ParticleSystem::tick() {
    Componentable::tick();
    for (int i = 0; i < numParticles; i++) { 
        particles[i].duration -= watch.getTime();
    }
    duration -= watch.getTime();
    watch.resetTime(); 
}
 
int ParticleSystem::getNumParticles() { 
    return numParticles;
}

Particle& ParticleSystem::getNthParticle(int n) { 
    return particles[n];
}

void ParticleSystem::setWorld(World *world_) {
    world  = world_;
}

Particle* ParticleSystem::getParticles() {
    return particles;
}
const glm::vec3& ParticleSystem::getPos() {
    return posVec;
}
 
 
