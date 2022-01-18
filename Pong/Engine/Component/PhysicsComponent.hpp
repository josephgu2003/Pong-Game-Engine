//
//  PhysicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/16/21.
//

#ifndef PhysicsComponent_hpp
#define PhysicsComponent_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include "Component.hpp"

class Actor;
class World;

// handles gravity for now, also stops falling thru the world
class PhysicsComponent : public Component {
    friend class World;
protected:
    bool gravityOn = true;
    virtual void handleGravity(Actor& actor);
public:
    PhysicsComponent(Actor& actor, bool gravityOn_);
    virtual void tick() override;
};

#endif /* PhysicsComponent_hpp */
