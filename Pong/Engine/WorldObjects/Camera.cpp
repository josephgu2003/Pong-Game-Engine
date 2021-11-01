//
//  Camera.cpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#include "Camera.hpp"
#include "Actor.hpp"
#include <stdio.h>

Camera::Camera() {
    posVec = glm::vec3 (0.0f,0.0f,0.0f);
    eulerAngles = glm::vec3(0.0f, -90.0f, 0.0f);
}

Camera::Camera(Actor* actor_) {
    setActor(actor_);
    posVec = glm::vec3 (0.0f,0.0f,0.0f);
    eulerAngles = glm::vec3(0.0f, -90.0f, 0.0f);
    updateVecs();
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
    dirVec = glm::normalize(dirVec);
    rightVec = glm::cross(glm::vec3(0,1,0),dirVec);
    rightVec = glm::normalize(rightVec);
    upVec = glm::cross(dirVec,rightVec); 
    upVec = glm::normalize(upVec);
    posVec = actor->getPos()-dirVec*5.0f+actor->camFocus+glm::vec3(0.0,1.0,0);
    if (posVec.y < -0.11) {
        posVec.x += (-0.11f-posVec.y)*dirVec.x/dirVec.y;
        posVec.z += (-0.11f-posVec.y)*dirVec.z/dirVec.y;
        posVec.y += (-0.11f-posVec.y);  
    }
}
  
void Camera::orientActor() {
    float yaw = eulerAngles.y;
    if (actor) actor->orient(yaw);
}

void Camera::tick() {
    float yaw = eulerAngles.y;
    if (actor)  actor->orient(yaw);
    updateVecs();
}
