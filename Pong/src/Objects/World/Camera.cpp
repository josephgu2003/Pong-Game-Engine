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
    pitch=0.0f;
    yaw=-90.0f;
    roll=0.0f;
}

Camera::Camera(Actor* actor_) {
    setActor(actor_);
    posVec = glm::vec3 (0.0f,0.0f,0.0f);
    pitch=0.0f;
    yaw=-90.0f;
    roll=0.0f;
    updateVecs();
}

Camera::~Camera() {
    
}

void Camera::setActor(Actor* actor_) {
    actor = actor_;
}
float Camera::getX() {
    return posVec.x;
}
float Camera::getY() {
    return posVec.y;
}
float Camera::getZ() {
    return posVec.z;
}
float Camera::getPitch() {
    return pitch;
}
float Camera::getYaw() {
    return yaw;
}
float Camera::getRoll() {
    return roll;
}
void Camera::incPitch(float inc) {
    if (std::abs(pitch + inc) < 70) {
        pitch += inc;
    }
}
void Camera::incYaw(float inc) {
    yaw += inc;
}
void Camera::incRoll(float inc) {
    roll += inc;
}

void Camera::updateVecs() { //updates vecs, keeps correct positioning relative to player char, so this function is meant to be called in 3rd person mode
    dirVec.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    dirVec.y = std::sin(glm::radians(pitch));
    dirVec.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    dirVec = glm::normalize(dirVec);
    rightVec = glm::cross(glm::vec3(0,1,0),dirVec);
    rightVec = glm::normalize(rightVec);
    upVec = glm::cross(dirVec,rightVec);
    upVec = glm::normalize(upVec);
    posVec = actor->posVec-dirVec*3.0f+actor->camFocus;
    if (posVec.y < -0.11) {
        posVec.x += (-0.11f-posVec.y)*dirVec.x/dirVec.y;
        posVec.z += (-0.11f-posVec.y)*dirVec.z/dirVec.y;
        posVec.y += (-0.11f-posVec.y);
    }
}

void Camera::orientActor() {
    actor->orient(yaw);
}

void Camera::tick() {
    actor->orient(yaw);
    updateVecs();
}
