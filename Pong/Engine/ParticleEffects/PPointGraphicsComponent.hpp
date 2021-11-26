//
//  PPointGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PPointGraphicsComponent_hpp
#define PPointGraphicsComponent_hpp

#include <stdio.h>
#include "PGraphicsComponent.hpp"

class PPointGraphicsComponent : public PGraphicsComponent {
    public:
    PPointGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const Material& map);
    virtual void tick() override;
};


#endif /* PPointGraphicsComponent_hpp */

