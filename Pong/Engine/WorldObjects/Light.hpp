//
//  Light.hpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Light {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
public:
    Light();
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    const glm::vec3& getAmbient() const;
    const glm::vec3& getDiffuse() const;
    const glm::vec3& getSpecular() const;
};

#endif /* Light_hpp */
