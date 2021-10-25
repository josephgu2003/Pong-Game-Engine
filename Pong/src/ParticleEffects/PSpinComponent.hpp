//
//  PSpinComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/23/21.
//

#ifndef PSpinComponent_hpp
#define PSpinComponent_hpp

#include <stdio.h>
#include "ParticleComp.hpp"
#include <glm/glm.hpp>

class ParticleSystem;

class PSpinComponent : public ParticleComp {
private:
    Particle* particles = NULL;
    float numParticles;
    glm::vec3 spin;
public:
    PSpinComponent(ParticleSystem& pe, glm::vec3 spin_);
    virtual void tick() override;
};
#endif /* PSpinComponent_hpp */
