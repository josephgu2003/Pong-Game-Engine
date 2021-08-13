//
//  Component.hpp
//  Pong
//
//  Created by Joseph Gu on 5/29/21.
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>

class Actor;
class World;

enum CompType {
    COMBAT,
    ANIM,
    PHYSICS,
    GRAPHICS
};

class Component {
private:
protected:
    CompType type;
public:
    Component();
    CompType getType();
    virtual void tick(Actor& actor, World& world) = 0;
};

#endif /* Component_hpp */
