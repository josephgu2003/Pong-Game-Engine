//
//  DirlightTransition.hpp
//  Pong
//
//  Created by Joseph Gu on 1/22/22.
//

#ifndef DirlightTransition_hpp
#define DirlightTransition_hpp

#include <stdio.h>
#include "Component.hpp"
#include <glm/glm.hpp>
#include "Watch.hpp"
#include "DirectionalLight.hpp"

class World;

class DirlightTransition : public Component {
private:
    Watch watch;
    float timeTillDone;
    DirectionalLight newLight;
    DirectionalLight oldLight;
public:
    DirlightTransition(World& world, float timeTillDone, const DirectionalLight& newLight);
    void tick() override;
};
#endif /* DirlightTransition_hpp */
