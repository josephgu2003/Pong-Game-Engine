//
//  InkGlyphs.hpp
//  Pong
//
//  Created by Joseph Gu on 6/13/21.
//

#ifndef InkGlyphs_hpp
#define InkGlyphs_hpp


#include <stdio.h>
#include "Particle.hpp"
#include "ComplexParticles.hpp"

class InkGlyphs : public ParticleEffect {
public:
    virtual void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, float ptcPerSec_, float duration) override;
    virtual void tick() override;
    virtual void refreshParticle() override;
};

#endif /* InkGlyphs_hpp */
