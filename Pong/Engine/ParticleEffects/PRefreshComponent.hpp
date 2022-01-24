//
//  PRefreshComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PRefreshComponent_hpp
#define PRefreshComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Watch.hpp"
#include "Dice.hpp"
#include <glm/glm.hpp>

class ParticleSystem;

struct Particle; 

class PRefresh : public Component {
private:
    Watch myWatch;
    int ptcPerInterval;
    float refreshInterval;
protected:
    Dice mainDice;
    int firstUnused;
    float particleMaxDuration;
    Particle* particles = NULL;
    glm::vec3* velocities = NULL;
    int numParticles;
    bool refreshVel;
    virtual void refreshParticle() = 0;
    glm::vec3 particleOrigin;
public:
    PRefresh(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond, float refreshInterval);
    virtual void tick() override;
    void refreshAll();
    float getParticleLifetime();
};

class PRefreshComponent : public PRefresh {
private:
    glm::vec3 dimensions;
    glm::vec3 velRangeLow;
    glm::vec3 velRangeHigh;
protected:
    void refreshParticle() override;
public:
    PRefreshComponent(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond, float refreshInterval, glm::vec3 dimensions, glm::vec3 velRangeLow, glm::vec3 velRangeHigh);
    ~PRefreshComponent() {
         
    }
};


#endif /* PRefreshComponent_hpp */
