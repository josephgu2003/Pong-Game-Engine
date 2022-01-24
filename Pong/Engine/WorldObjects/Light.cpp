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

const glm::vec3& Light::getAmbient() const {
    return ambient;
}
const glm::vec3& Light::getDiffuse() const {
    return diffuse;
}
const glm::vec3& Light::getSpecular() const {
    return specular;
}
