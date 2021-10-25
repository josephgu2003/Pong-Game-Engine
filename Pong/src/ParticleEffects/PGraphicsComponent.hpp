//
//  PGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/20/21.
//

#ifndef PGraphicsComponent_hpp
#define PGraphicsComponent_hpp

#include <stdio.h>
#include "Renderable.hpp"
#include "ParticleComp.hpp"

class PGraphicsComponent : public Renderable, public ParticleComp {
public:
    PGraphicsComponent(ParticleSystem& pe, float size, Shader* shader, const TextureMaps& map);
    virtual void draw(Renderer* r) override;
    virtual void tick() override;
    void setColor(float r, float g, float b);
};

#endif /* PGraphicsComponent_hpp */
