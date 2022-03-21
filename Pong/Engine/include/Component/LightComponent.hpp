//
//  LightComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 1/23/22.
//

#ifndef LightComponent_hpp
#define LightComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "PointLight.hpp"
#include "Positionable.hpp"
#include "Renderer.hpp"
// possibly use visitor pattern, let bindUniform call be in the Light derived class

class LightComponent : public Component {
    PointLight pointLight; 
    Positionable& following; // ehh??
public:
    LightComponent(Componentable& c, Positionable& p, const PointLight& pl) : Component(c), pointLight(pl), following(p) {
    }
    void tick() override { 
        pointLight.pos = (following.getPos());
    }
    void updateRenderer(Renderer* r) {
        r->updateLight(pointLight); // ehhhh static members?
    }
};

#endif /* LightComponent_hpp */
