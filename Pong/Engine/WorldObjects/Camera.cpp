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
    lockOnActor = true;
    needRotate = false;
}

Camera::~Camera() { 
    
}

void Camera::lockOntoActor() {
    lockOnActor  = true;
}

void Camera::setActor(Positionable* actor_) {
    actor = actor_;
}

void Camera::updateVecs() { //updates vecs, keeps correct positioning relative to player char, so this function is meant to be called in 3rd person mode
    if (!lockOnActor || !actor) {
        return; 
    } 
    setPos(0.5f*actor->getDir()+actor->getPos()+CAM_OFFSET-3.0f*glm::vec3(actor->getDir().x, getDir().y, actor->getDir().z));
    orientYawTo(actor->getDir()); // align with actor
           
    float h = actor->getPos().y;
    glm::vec3 posVec = getPos();
    glm::vec3 dirVec = getDir();  
    if (posVec.y < h) {
        posVec.x += (h-posVec.y)*dirVec.x/dirVec.y;
        posVec.z += (h-posVec.y)*dirVec.z/dirVec.y; 
        posVec.y += (h-posVec.y);
    }
    setPos(posVec);
} 
  
void Camera::tick() {
    //rotate then orient actor then set pos relative to actor
    
}
   
void Camera::unlockFromActor() {
    lockOnActor = false;
}
 
void Camera::unlockFromActor(glm::vec3 pos, glm::vec3 dir) {
    unlockFromActor();
    setPos(pos);
    orientYawTo(dir);
}
