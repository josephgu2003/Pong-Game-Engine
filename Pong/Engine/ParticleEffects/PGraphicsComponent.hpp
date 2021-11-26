//
//  PGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 10/20/21.
//

#ifndef PGraphicsComponent_hpp
#define PGraphicsComponent_hpp 

#include <stdio.h>
#include "GraphicsObject.hpp"
#include "Component.hpp"

class ParticleSystem;

class PGraphicsComponent : public GraphicsObject, public Component {
public:
    PGraphicsComponent(ParticleSystem& pe, float size, Shader* shader, const Material& map);
    virtual void draw(Renderer* r) override;
    virtual void tick() override;
    void setColor(float r, float g, float b);
};

#endif /* PGraphicsComponent_hpp */
