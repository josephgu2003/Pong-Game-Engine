//
//  Component.hpp
//  Pong
//
//  Created by Joseph Gu on 5/29/21.
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>
#include <memory>

enum CompType {
    AI,
    CHAR,
    LIFE,
    NAME,
    COMBAT,
    PARTICLESPIN,
    ANIM,
    PHYSICS,
    GRAPHICS
};

class Componentable; 

class Component {
protected:
    Componentable* actor = nullptr;
    CompType type;
public:
    Component(Componentable& compUser_) {
        actor = &compUser_;
    }

    CompType getType() {
        return type;
    }

    virtual void tick() = 0;
};

#endif /* Component_hpp */
