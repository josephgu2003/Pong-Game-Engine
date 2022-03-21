//
//  FogTransition.hpp
//  Pong
//
//  Created by Joseph Gu on 1/21/22.
//

#ifndef FogTransition_hpp
#define FogTransition_hpp

#include <stdio.h>
#include "Component.hpp"
#include <glm/glm.hpp>
#include "Watch.hpp"

class World;

class FogTransition : public Component {
private:
    Watch watch;
    float timeTillDone;
    float fogDensity;
    float fogGradient;
    glm::vec3 fogColor;
    
    float fogDensity0;
    float fogGradient0;
    glm::vec3 fogColor0;
public:
    FogTransition(World& world, float timeTillDone, float fogDensity, float fogGradient, glm::vec3 fogColor);
    void tick() override;
};
#endif /* FogTransition_hpp */
