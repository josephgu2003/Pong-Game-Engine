//
//  Camera.cpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#include "Camera.hpp"
#include "Actor.hpp"
#include <stdio.h>
#include "World.hpp"

#define CAM_OFFSET glm::vec3(0,0.3,0)

Camera::Camera() : Positionable() {
}

Camera::~Camera() { 
    
}

void Camera::setActor(Actor* actor_) {
    actor = actor_;
}

void Camera::updateVecs() { //updates vecs, keeps correct positioning relative to player char, so this function is meant to be called in 3rd person mode
    float yaw = eulerAngles.y;
    float pitch = eulerAngles.x;
    dirVec.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    dirVec.y = std::sin(glm::radians(pitch));
    dirVec.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    Positionable::GramSchmidtAndNormalizeOrientations();
    if (!actor) {
        return;  
    }
    posVec = actor->getPos()-dirVec*3.0f+CAM_OFFSET+actor->getDir();
    float h = actor->getPos().y;
    if (posVec.y < h) {
        posVec.x += (h-posVec.y)*dirVec.x/dirVec.y;
        posVec.z += (h-posVec.y)*dirVec.z/dirVec.y; 
        posVec.y += (h-posVec.y);
    }
}
  
void Camera::rotate(glm::vec3 angles) {
    eulerAngles += angles;
}
 
void Camera::tick() {
    float yaw = eulerAngles.y;
    if (actor)  actor->orient(yaw);
}
 
