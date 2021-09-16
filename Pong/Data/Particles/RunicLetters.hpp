//
//  RunicLetters.hpp
//  Pong
//
//  Created by Joseph Gu on 9/13/21.
//

#ifndef RunicLetters_hpp
#define RunicLetters_hpp

#include <stdio.h>
#include "Particle.hpp"

class RunicLetters : public ParticleEffect {
public:
    virtual void setGraphics(Shader& shader) override;
    virtual void refreshParticle() override;
    virtual void tick() override;
};
#endif /* RunicLetters_hpp */
