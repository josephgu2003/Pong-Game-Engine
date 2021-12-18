//
//  Positionable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "Positionable.hpp"
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>

Positionable::Positionable() {
    posVec.x = 0.0f;
    posVec.y = 0.0f;
    posVec.z = 0.0f;
    dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
    rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    upVec = glm::cross(dirVec, rightVec);
    eulerAngles = glm::vec3(0,0,0);
    offsetAngles = glm::vec3(0);
}

void Positionable::orient(float yaw_) {
    eulerAngles.y = yaw_;
    float angle = glm::radians(eulerAngles.y+offsetAngles.y);
    dirVec.x = std::cos(angle);
    dirVec.z = std::sin(angle);
    GramSchmidtAndNormalizeOrientations();
}

void Positionable::turnTowards(const glm::vec3& newDir_) {

    float dYaw = (180.0f/3.14159)* glm::orientedAngle(glm::normalize(glm::vec2(newDir_.x,newDir_.z)), glm::normalize(glm::vec2(dirVec.x,dirVec.z))); // rotates such that dirVec points

   /** eulerAngles -= glm::vec3(0,dYaw,0);
    if (eulerAngles.x < -45.0f) {
        eulerAngles.x = -45.0f;
    } 

    dirVec = newDir_;
    
    GramSchmidtAndNormalizeOrientations();**/
    orient(eulerAngles.y-dYaw); 
     
}

void Positionable::setPos(glm::vec3 pos_) {
    posVec = pos_;
    Subject::notifyAll(SUBJ_POS_CHANGED);
}

void Positionable::translatePos(const glm::vec3& translate) {
    posVec += translate;
    notifyAll(SUBJ_POS_CHANGED);
}

void Positionable::setPosY(float y_) {
    posVec.y = y_;
    notifyAll(SUBJ_POS_CHANGED);
}
void Positionable::posDir(float speed) { 
    posVec += speed * dirVec;
    notifyAll(SUBJ_POS_CHANGED);
}

void Positionable::posRight(float speed) {
    posVec += speed * rightVec;
    notifyAll(SUBJ_POS_CHANGED);
}

void Positionable::randomPosAround(glm::vec3 pivot) {

}

void Positionable::rotate(glm::vec3 eulers) {
    eulerAngles += eulers;
    orient(eulerAngles.y);
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
  
float Positionable::getDistanceTo(glm::vec3 p) {
    float d = glm::length(posVec - p); 
    return d;
}
void Positionable::offsetOrientationVectors(glm::vec3 eulers) {
    offsetAngles = eulers;
}

void Positionable::turnTowards(Positionable* p) {
    turnTowards(p->getPos()-posVec);
}

void Positionable::GramSchmidtAndNormalizeOrientations() { // finds rightVec and normalizes DirVec
    dirVec = glm::normalize(dirVec); 
    rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    rightVec = glm::normalize(rightVec);
    upVec = glm::cross(dirVec,rightVec);
    upVec = glm::normalize(upVec);
}

glm::vec3 Positionable::getDir() {
    return dirVec; 
}
/**
#include <glm/gtx/vector_angle.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
modelMat = glm::mat4(1.0f);
modelMat = glm::translate(modelMat, posVec);
glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(90.0f-eulerAngles.y),glm::radians(eulerAngles.z));
glm::quat MyQuaternion = glm::quat(rotations);

glm::mat4 RotationMatrix = toMat4(MyQuaternion);
modelMat = modelMat * RotationMatrix;
**/
