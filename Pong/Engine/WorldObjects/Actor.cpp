//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"
#include <stdio.h>
#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.hpp"
 

#define JUMP_SPEED 0.1f

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
    
    graphics->getShader()->setUniform("modelMat", getModelMatrix());
    
    glm::mat3 transposeInverse = glm::mat3(glm::transpose(glm::inverse(getModelMatrix())));
    graphics->getShader()->setUniform("transposeInverseModelMat", transposeInverse);
}

void Actor::posDir(float speed) { 
    if (state != STATE_PARALYZED) {
        Positionable::posDir(speed);
    }
}

void Actor::posRight(float speed) {
    if (state != STATE_PARALYZED) {
        Positionable::posRight(speed);
    }
}


void Actor::jump() {
    if (state != STATE_PARALYZED && state!= STATE_JUMPING) {
    velVec.y = JUMP_SPEED;
    state = STATE_JUMPING;
    }
}

void Actor::velRight(float speed) {
    velVec += getRight()*speed;
}
void Actor::velDir(float speed) {
    velVec += getDir()*speed;
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
    if (dynamic_pointer_cast<GraphicsComponent>(comp)) graphics = static_pointer_cast<GraphicsComponent>(comp);
}
   
 
 
