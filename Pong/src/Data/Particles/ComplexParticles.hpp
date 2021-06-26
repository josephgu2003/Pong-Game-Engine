//
//  ComplexParticles.hpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#ifndef ComplexParticles_hpp
#define ComplexParticles_hpp

#include <stdio.h>
#include "Particle.hpp"

class ComplexParticles : public ParticleEffect {
    virtual void refreshParticle() override;
public:
    virtual void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, float ptcPerSec_,float duration_) override;
};

#endif /* ComplexParticles_hpp */
