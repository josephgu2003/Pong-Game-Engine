//
//  PPointGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PPointGraphicsComponent_hpp
#define PPointGraphicsComponent_hpp

#include <stdio.h>
#include "GraphicsComponent.hpp"

class ParticleSystem; 

class PPointGraphicsComponent : public GraphicsComponent {
    private:
    float size;
    public:
    PPointGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const Material& map);
    ~PPointGraphicsComponent();
    virtual void tick() override;
};


#endif /* PPointGraphicsComponent_hpp */

