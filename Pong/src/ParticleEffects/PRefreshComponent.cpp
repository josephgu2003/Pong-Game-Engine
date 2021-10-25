//
//  PRefreshComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#include "PRefreshComponent.hpp"
#include "Particle.hpp"
#include "PPhysicsComponent.hpp"

PRefreshComponent::PRefreshComponent(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond_, float refreshInterval_, glm::vec3 dimensions_, glm::vec3 velRangeLow_, glm::vec3 velRangeHigh_) : ParticleComp(pe){
    ptcPerSecond = ptcPerSecond_;
    refreshInterval = refreshInterval_;
    myWatch.resetTime();
    firstUnused = 0;
    particles = pe.getParticles();
    numParticles = pe.getNumParticles();
    dimensions = dimensions_;
    type = PHYSICS;
    ptcPerInterval = std::ceil(ptcPerSecond * refreshInterval);
    particleMaxDuration = particleMaxDuration_;
    mainDice.setRange(0, 1000);
    velRangeLow = velRangeLow_;
    velRangeHigh = velRangeHigh_;
    
    refreshVel = true;
    if (glm::length(velRangeLow-velRangeHigh) == 0) {
        refreshVel = false;
    }
    
    hasPhysics = false;
} 

void PRefreshComponent::tick() {
    ppc = actor->getComponent<PPhysicsComponent>();
    if (ppc) {
        velocities = ppc->getVelocities();
    }
    if (myWatch.getTime() > refreshInterval) { // every 0.2 sec do a refresh
        myWatch.resetTime();
        
        for (int i = 0; i < ptcPerInterval; i++) {
            refreshParticle();
        }
    }

}

void PRefreshComponent::refreshParticle() {
    glm::vec3 displacement;
    
    for (int i = 0; i < 3; i++) {
        displacement[i] = dimensions[i]*(-0.5f + 0.01*(mainDice.roll() % 100)); 
    }
    
    if (refreshVel && ppc) {
        for (int i = 0; i < 3; i++) {
            velocities[firstUnused][i] = (velRangeHigh[i]-velRangeLow[i])*(0.01*(mainDice.roll() % 100)) + velRangeLow[i];
        }
    }
    
    particles[firstUnused].posVec = actor->getPos() + displacement;
    
    particles[firstUnused].pyrAngles = glm::vec3(0,(mainDice.roll() % 180),0);
     
    particles[firstUnused].duration = particleMaxDuration;
    
    if(firstUnused == (numParticles-1)) {
        firstUnused = 0;
    } else if (particles[firstUnused+1].duration<=0) {
        firstUnused++;
    }
 
} 
