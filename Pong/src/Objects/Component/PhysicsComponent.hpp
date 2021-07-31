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

class PhysicsComponent : public Component {
    friend class World;
private:
    CompType type = PHYSICS;
protected:
    bool gravityOn = true;
    virtual void handleGravity(Actor& actor);
public:
    PhysicsComponent(bool gravityOn_);
    virtual void tick(Actor& actor, World& world) override;
};

#endif /* PhysicsComponent_hpp */
