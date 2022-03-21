//
//  SkyColorTransition.hpp
//  Pong
//
//  Created by Joseph Gu on 3/18/22.
//

#ifndef SkyColorTransition_hpp
#define SkyColorTransition_hpp

#include <stdio.h>
#include "Component.hpp"
#include <glm/glm.hpp>
#include "Watch.hpp"

class World;

class SkyColorTransition : public Component {
private:
    Watch watch;
    float timeTillDone;
    
    glm::vec3 oldColor;
    
    glm::vec3 newColor;
public:
    SkyColorTransition(World& world,  float timeTillDone_, const glm::vec3& newColor);
    void tick() override;
};
#endif /* SkyColorTransition_hpp */
