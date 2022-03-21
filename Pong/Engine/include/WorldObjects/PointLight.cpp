//
//  PointLight.cpp
//  Pong
//
//  Created by Joseph Gu on 1/23/22.
//

#include "PointLight.hpp"
#include "Positionable.hpp"

PointLight::PointLight(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, float constant_, float linear_, float quadratic_, glm::vec3 pos_) : Light(ambient_, diffuse_, specular_){
    constant = constant_;
    linear = linear_;
    quadratic = quadratic_;
    pos = pos_;
}

void PointLight::notify(const Subject &subject, GameEvent ge) {
    if (ge == SUBJ_POS_CHANGED) {
        pos = static_cast<const Positionable&>(subject).getPos(); // static cast could fail
    }
}

