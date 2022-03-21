//
//  DirlightTransition.cpp
//  Pong
//
//  Created by Joseph Gu on 1/22/22.
//

#include "DirlightTransition.hpp"
#include "World.hpp"

DirlightTransition::DirlightTransition(World& world, float timeTillDone_, const DirectionalLight& newLight_) : Component(world) {
    timeTillDone = timeTillDone_;
    newLight = newLight_;
    watch.resetTime();
    oldLight = world.getDirectionalLight();
}
 
void DirlightTransition::tick() {
    float ratio = watch.getTime() / timeTillDone;
    if (ratio <= 1.0f) {
        World* w = static_cast<World*>(actor);
        w->setDirectionalLight(oldLight.interpolateWith(newLight, ratio));
    } else {
        static_cast<World*>(actor)->deleteComponent(this);
    }
}
 
