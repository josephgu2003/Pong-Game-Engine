//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"
#include <stdio.h>
#include <string>
#include "AssetManager.hpp"
#include "PhysicsComponent.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CombatComponent.hpp"
#include "AnimComponent.hpp" 
#include "NameComponent.hpp"
#include "LifeComponent.hpp"
#include "Renderer.hpp"
#include "AIComponent.hpp"
#include "Shader.hpp"
 

#define JUMP_SPEED 0.05f

Actor::Actor() : Positionable() {
    state = STATE_IDLE;
    velVec = glm::vec3(0.0f, 0.0f, 0.0f); 
} 
 
Actor::~Actor() {
 
} 



World& Actor::getWorld() {
    return *world;
}


void Actor::tick() {
    Componentable::tick();
    
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, posVec);
    glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(90.0f-eulerAngles.y),glm::radians(eulerAngles.z));
    glm::quat MyQuaternion = glm::quat(rotations);
    
    glm::mat4 RotationMatrix = toMat4(MyQuaternion);
    modelMat = modelMat * RotationMatrix;
    
    graphics->getShader()->setMat4("modelMat", modelMat);
    
    glm::mat3 transposeInverse = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    graphics->getShader()->setMat3("transposeInverseModelMat", transposeInverse);
}

void Actor::posDir(float speed) {
    if (state != STATE_PARALYZED) {
    posVec += speed * dirVec;
    }
}

void Actor::posRight(float speed) {
    if (state != STATE_PARALYZED) {
    posVec += speed * rightVec;
    }
}


void Actor::jump() {
    if (state != STATE_PARALYZED && state!= STATE_JUMPING) {
    velVec.y = JUMP_SPEED;
    state = STATE_JUMPING;
    }
}

void Actor::velRight(float speed) {
    velVec += rightVec*speed;
}
void Actor::velDir(float speed) {
    velVec += dirVec*speed;
}

glm::vec3 Actor::getVel() {
    return velVec;
}

void Actor::setVel(glm::vec3 velVel_) {
    velVec = velVel_;
}

void Actor::accelerate(glm::vec3 accel) {
    velVec += accel;
}


void Actor::setWorld(World* world_) {
    world = world_;
}

void Actor::setState(State state_) {
    state = state_;
}
State Actor::getState() {
    return state; 
}

void Actor::addComp(const std::shared_ptr<Component>& comp) {
    Componentable::addComp(comp);
    if (comp->getType() == GRAPHICS) graphics = static_pointer_cast<GraphicsComponent>(comp);
}
  
 
