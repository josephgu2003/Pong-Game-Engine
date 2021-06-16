//
//  ProximityComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#ifndef ProximityComponent_hpp
#define ProximityComponent_hpp

#include <stdio.h>
#include "Component.hpp"

class Actor;
class World;

class ProximityComponent : public Component {
    friend class World;
protected:
    Actor* biggestTarget;
    bool somethingNear = false;
public:
    virtual void tick(Actor& actor, World& world);
    virtual void trigger(Actor& actor, World& world) = 0;
};
#endif /* ProximityComponent_hpp */
