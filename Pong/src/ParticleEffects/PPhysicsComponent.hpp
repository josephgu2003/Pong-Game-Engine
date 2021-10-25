//
//  PPhysicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/22/21.
//

#ifndef PPhysicsComponent_hpp
#define PPhysicsComponent_hpp

#include <stdio.h>
#include "ParticleComp.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "Force.hpp"

class ParticleSystem;

class PPhysicsComponent : public ParticleComp {
private:
    Particle* particles = NULL;
    glm::vec3* velocities = NULL;
    std::vector<std::unique_ptr<Force>> forces;
    int numParticles;
    float gravity;
    float friction;
public:
    PPhysicsComponent(ParticleSystem& pe, float gravity, float friction);
    virtual void tick() override;
    glm::vec3* getVelocities();
};


#endif /* PPhysicsComponent_hpp */
