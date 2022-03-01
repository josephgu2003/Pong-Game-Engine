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
    state = CAM_FREE;
    needRotate = false;
    interpolating = false;
    interpolationDuration = 1.0f;
    oldDir = glm::vec3(0.0f);
    oldPos = glm::vec3(0.0f);
}

Camera::~Camera() { 
    
}

void Camera::lockOntoActor() {
    state = CAM_FOLLOW_ACTOR_ALIGNED;
}

void Camera::setActor(Positionable* actor_) {
    actor = actor_;
    lockOntoActor();
}

void Camera::updateVecs() { //updates vecs, keeps correct positioning relative to player char, so this function is meant to be called in 3rd person mode
    if (!actor) {
        return;
    }
    switch (state) {
        case CAM_FREE: {
            return;
        }
            
        case CAM_FOLLOW_ACTOR_ALIGNED: {
            float h = actor->getPos().y;
            orientYawTo(actor->getDir());
            glm::vec3 posVec = (actor->getPos()+CAM_OFFSET - 3.0f * getDir());
            glm::vec3 dirVec = actor->getDir();
            
            if (posVec.y < h) {
                posVec.x += (h-posVec.y)*dirVec.x/dirVec.y;
                posVec.z += (h-posVec.y)*dirVec.z/dirVec.y;
                posVec.y += (h-posVec.y);
            }
            
            if (interpolating) {
                float ratio = watch.getTime() / interpolationDuration;
                if (ratio > 1.0f) {
                    interpolating = false;
                    return;
                } 
                orientYawTo(glm::mix(oldDir, actor->getDir(), ratio));
                setPos(glm::mix(oldPos, posVec, ratio));
            } else {
                orientYawTo(actor->getDir()); // align with actor
                setPos(posVec);
            }
            break;
        }
            
        case CAM_FOLLOW_ACTOR_UNALIGNED: {
            float h = actor->getPos().y;
            glm::vec3 posVec = (actor->getPos()+CAM_OFFSET - 3.0f * getDir());
            glm::vec3 dirVec = actor->getDir();
            
            if (posVec.y < h) {
                posVec.x += (h-posVec.y)*dirVec.x/dirVec.y;
                posVec.z += (h-posVec.y)*dirVec.z/dirVec.y;
                posVec.y += (h-posVec.y);
            }
            
            if (interpolating) {
                setPos(posVec);
            } else {
                setPos(posVec);
            }
            break;
        }
    }
} 

void Camera::tick() {
    //rotate then orient actor then set pos relative to actor
    updateVecs();
}

void Camera::unlockFromActor() {
    state = CAM_FREE;
}

void Camera::unlockFromActor(glm::vec3 pos, glm::vec3 dir) {
    unlockFromActor();
    setPos(pos);
    orientYawTo(dir);
}

void Camera::setState(CameraState state_) {
    state = state_;
}
void Camera::setStateAndInterpolate(CameraState state_, float timeTo) {
    assert(abs(timeTo - 0.0f) > 0.001f);
    
    state = state_;
    interpolating = true;
    interpolationDuration = timeTo;
    watch.resetTime();
    oldDir = getDir();
    oldPos = getPos();
}
