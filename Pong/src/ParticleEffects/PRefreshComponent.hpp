//
//  PRefreshComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PRefreshComponent_hpp
#define PRefreshComponent_hpp

#include <stdio.h>
#include "ParticleComp.hpp"
#include "Watch.hpp"
#include "Dice.hpp"
#include <glm/glm.hpp>

class ParticleSystem;

class PPhysicsComponent;

class PRefreshComponent : public ParticleComp {
private:
    Watch myWatch; 
    Dice mainDice;
    int ptcPerSecond;
    int ptcPerInterval;
    float refreshInterval;
    int firstUnused;
    float particleMaxDuration;
    Particle* particles = NULL;
    glm::vec3* velocities = NULL;
    int numParticles;
    glm::vec3 dimensions;
    glm::vec3 velRangeLow;
    glm::vec3 velRangeHigh;
    bool refreshVel;
    bool hasPhysics;
    PPhysicsComponent* ppc = NULL;
public:
    PRefreshComponent(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond, float refreshInterval, glm::vec3 dimensions, glm::vec3 velRangeLow, glm::vec3 velRangeHigh);
    virtual void tick() override;
    void refreshParticle();
};


#endif /* PRefreshComponent_hpp */
