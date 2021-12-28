//
//  ParticleFactory.hpp
//  Pong
//
//  Created by Joseph Gu on 11/4/21.
//

#ifndef ParticleFactory_hpp
#define ParticleFactory_hpp

#include <stdio.h>
#include "Particle.hpp"

enum ParticleEffectSeed {
    PE_FIREWORKS,
    PE_MIST,
    PE_BODYSPARKS,
    PE_RUNICLETTERS,
    PE_SNOW,
    PE_MOONBELL_SWORDS
};

class ParticleFactory {
public:
    virtual std::shared_ptr<ParticleSystem> makeParticles(ParticleEffectSeed PESeed, glm::vec3 posVec_);
};


#endif /* ParticleFactory_hpp */
