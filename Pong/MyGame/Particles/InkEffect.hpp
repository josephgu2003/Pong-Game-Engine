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

class InkEffect : public ParticleSystem {
public:
    virtual void setGraphics(Shader* shader) override;
    virtual void refreshParticle() override;
    virtual void setActor(Actor* actor) override;
};

#endif /* InkGlyphs_hpp */
