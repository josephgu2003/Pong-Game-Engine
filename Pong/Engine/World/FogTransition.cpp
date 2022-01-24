//
//  FogTransition.cpp
//  Pong
//
//  Created by Joseph Gu on 1/21/22.
//

#include "FogTransition.hpp"
#include "World.hpp"

FogTransition::FogTransition(World& world, float timeTillDone_, float fogDensity_, float fogGradient_, glm::vec3 fogColor_) : Component(world) {
    timeTillDone = timeTillDone_;
    fogDensity = fogDensity_;
    fogGradient = fogGradient_;
    fogColor = fogColor_;
    watch.resetTime();
    
    WorldFog& df = world.getDistanceFog();
    fogDensity0 = df.fogDensity;
    fogGradient0 = df.fogGradient;
    fogColor0 = df.fogColor;
}
 
void FogTransition::tick() { 
    float ratio = watch.getTime() / timeTillDone;
    if (ratio <= 1.0f) {
        WorldFog& df = static_cast<World*>(actor)->getDistanceFog();
        df.setDistanceFog(glm::mix(fogDensity0,fogDensity,ratio), glm::mix(fogGradient0,fogGradient,ratio), glm::mix(fogColor0,fogColor,ratio));
    } else {
        static_cast<World*>(actor)->deleteComponent(this);
    }
}
 
