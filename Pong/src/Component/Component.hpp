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

class Actor;
class World;

enum CompType {
    LIFE,
    NAME,
    COMBAT,
    ANIM,
    PHYSICS,
    GRAPHICS
};

class Component {
protected:
    CompType type;
    Actor* actor = NULL;
public:
    Component();
    Component(Actor& actor);
    CompType getType();
    virtual void tick() = 0;
};

#endif /* Component_hpp */
