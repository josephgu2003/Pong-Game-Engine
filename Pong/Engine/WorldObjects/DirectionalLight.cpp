//
//  DirectionalLight.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() {
    
}
DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 shineDir_) : Light::Light(ambient,diffuse,specular){
    shineDir = shineDir_;
}
void DirectionalLight::setShineDir(glm::vec3 shineDir_) {
    shineDir = shineDir_;
}
glm::vec3 DirectionalLight::getShineDir() const {
    return shineDir;
} 

DirectionalLight DirectionalLight::interpolateWith(DirectionalLight& dl, float ratio) {
    return DirectionalLight(glm::mix(getAmbient(), dl.getAmbient(), ratio), glm::mix(getDiffuse(), dl.getDiffuse(), ratio), glm::mix(getSpecular(), dl.getSpecular(), ratio), glm::mix(getShineDir(), dl.getShineDir(), ratio));
}
