//
//  Light.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "Light.hpp"

Light::Light() {
    
}
Light::Light(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) {
    ambient = ambient_;
    diffuse = diffuse_;
    specular = specular_;
}

glm::vec3 Light::getAmbient() {
    return ambient;
}
glm::vec3 Light::getDiffuse() {
    return diffuse;
}
glm::vec3 Light::getSpecular() {
    return specular;
}
