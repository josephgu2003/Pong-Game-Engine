//
//  Prop.hpp
//  Pong
//
//  Created by Joseph Gu on 11/11/21.
//

#ifndef Prop_hpp
#define Prop_hpp

#include "Componentable.hpp"
#include "Positionable.hpp"
#include <stdio.h>

// anything in the world thats not alive
// for lots of grass, lod : render close ones, instance buffer

class World;

class Prop : public Componentable, public Positionable { // any character really
protected:
    World* world = NULL;
public:
    Prop();
    virtual void tick();
    
    void setWorld(World* world_);
    World& getWorld();
};

#endif /* Prop_hpp */
