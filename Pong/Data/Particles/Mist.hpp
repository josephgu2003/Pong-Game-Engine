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

class Mist : public ParticleEffect {
public:
    virtual void setGraphics() override;
    virtual void refreshParticle() override;
};

#endif /* Leaves_hpp */
