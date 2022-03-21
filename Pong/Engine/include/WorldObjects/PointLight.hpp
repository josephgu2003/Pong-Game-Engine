//
//  PointLight.hpp
//  Pong
//
//  Created by Joseph Gu on 1/23/22.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <stdio.h>
#include "Light.hpp"
#include "Observer.hpp" 

class PointLight : public Light, public Observer {
public:
    float constant;
    float linear;
    float quadratic;
    glm::vec3 pos;

    PointLight(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, float constant, float linear, float quadratic, glm::vec3 pos);
    
    void notify(const Subject &subject, GameEvent ge) override;
    
};

#endif /* PointLight_hpp */
