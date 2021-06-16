//
//  PhysicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/16/21.
//

#ifndef PhysicsComponent_hpp
#define PhysicsComponent_hpp

#include <stdio.h>

#include "Component.hpp"

class Actor;
class World;

class PhysicsComponent : public Component {
    friend class World;
protected:
public:
    virtual void tick(Actor& actor, World& world);
};

#endif /* PhysicsComponent_hpp */
