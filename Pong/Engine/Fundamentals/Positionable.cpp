//
//  Positionable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "Positionable.hpp"
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>

void Positionable::orient(float yaw_) {
    eulerAngles.y = yaw_;
    dirVec.x = std::cos(glm::radians(eulerAngles.y));
    dirVec.z = std::sin(glm::radians(eulerAngles.y));
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    dirVec = glm::normalize(dirVec);
    rightVec = glm::normalize(rightVec);
}

void Positionable::turnTowards(const glm::vec3& newDir_) {
    glm::vec3 newDir = glm::normalize(newDir_); 

    float dYaw = (180.0f/3.14159)* glm::orientedAngle(glm::normalize(glm::vec2(newDir.x,newDir.z)), glm::normalize(glm::vec2(dirVec.x,dirVec.z)));

    eulerAngles -= glm::vec3(0,dYaw,0);
    if (eulerAngles.x < -45.0f) {
        eulerAngles.x = -45.0f;
    }

    dirVec = newDir;
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    rightVec = glm::normalize(rightVec);
}

void Positionable::setPos(glm::vec3 pos_) {
    posVec = pos_;
}

void Positionable::translatePos(const glm::vec3& translate) {
    posVec += translate;
}

void Positionable::setPosY(float y_) {
    posVec.y = y_;
}
void Positionable::posDir(float speed) {
    posVec += speed * dirVec;
}

void Positionable::posRight(float speed) {
    posVec += speed * rightVec;
}

void Positionable::randomPosAround(glm::vec3 pivot) {

}

void Positionable::rotate(glm::vec3 eulers) {
    eulerAngles += eulers;
} 

float Positionable::getYaw() {
    return eulerAngles.y;
}

glm::vec3 Positionable::getPos() {
    return posVec;
}
  
float Positionable::getDistanceTo(Positionable* b) {
    float d = glm::length(posVec - b->getPos());
    return d;
}
