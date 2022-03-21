//
//  PQuadGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#ifndef PQuadGraphicsComponent_hpp
#define PQuadGraphicsComponent_hpp

#include <stdio.h>
#include "GraphicsComponent.hpp"

class ParticleSystem;
 
class PQuadGraphicsComponent : public GraphicsComponent {
private:
    float size;
    
    public:
    PQuadGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const Material& map);
    PQuadGraphicsComponent(ParticleSystem& pe, std::string modelFile, int numberParticles, float size, Shader* shader, const Material& map);
    ~PQuadGraphicsComponent();
    virtual void tick() override;
};

#endif /* PQuadGraphicsComponent_hpp */

