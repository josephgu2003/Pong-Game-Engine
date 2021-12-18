//
//  DirectionalLight.hpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include <stdio.h>
#include "Light.hpp"

class DirectionalLight : public Light {
    glm::vec3 shineDir;
public: 
    DirectionalLight();
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 shineDir);
    void setShineDir(glm::vec3 shineDir_);
    glm::vec3 getShineDir();
};
#endif /* DirectionalLight_hpp */
