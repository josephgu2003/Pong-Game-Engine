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

class InkEffect : public ParticleEffect {
public:
    virtual void setGraphics(Shader& shader) override;
    virtual void refreshParticle() override;
};

#endif /* InkGlyphs_hpp */
