//
//  PQuadGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PQuadGraphicsComponent_hpp
#define PQuadGraphicsComponent_hpp

#include <stdio.h>
#include "PGraphicsComponent.hpp"

class ParticleSystem;
 
class PQuadGraphicsComponent : public PGraphicsComponent {
    public:
    PQuadGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const TextureMaps& map);
    virtual void tick() override;
};

#endif /* PQuadGraphicsComponent_hpp */

