//
//  Leaves.hpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#ifndef Leaves_hpp
#define Leaves_hpp

#include <stdio.h>
#include "Particle.hpp"

class Leaves : public ParticleEffect {
public:
    void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
    void tick();
};

#endif /* Leaves_hpp */
