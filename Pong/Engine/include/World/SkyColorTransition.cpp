//
//  SkyColorTransition.cpp
//  Pong
//
//  Created by Joseph Gu on 3/18/22.
//

#include "SkyColorTransition.hpp"
#include "World.hpp"

SkyColorTransition::SkyColorTransition(World& world, float timeTillDone_, const glm::vec3& newColor_) : Component(world) {
    timeTillDone = timeTillDone_;
    watch.resetTime();
    
    oldColor = world.getAtmosphere().getSkyColor();
    newColor = newColor_;
}
 
void SkyColorTransition::tick() {
    float ratio = watch.getTime() / timeTillDone; 
    if (ratio <= 1.0f) {
        Atmosphere& atmosphere = static_cast<World*>(actor)->getAtmosphere();
        atmosphere.setSkyColor(glm::mix(oldColor, newColor, ratio));
    } else {
        static_cast<World*>(actor)->deleteComponent(this);
    }
}
 
