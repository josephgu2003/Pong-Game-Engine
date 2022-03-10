//
//  PhysicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/16/21.
//

#include "PhysicsComponent.hpp"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Actor.hpp"
#include "Component.hpp"
#include "World.hpp" 

PhysicsComponent::PhysicsComponent(Actor& actor, bool gravityOn_) : Component(actor) {
    gravityOn = gravityOn_;
    updatePriority = 1;
}  

void PhysicsComponent::tick() {
    auto ourActor = static_cast<Actor*>(actor);
    if (ourActor->getState() == STATE_IDLE || ourActor->getState() == STATE_JUMPING)
        ourActor->translatePos(ourActor->getVel());
    
    handleGravity(*ourActor);
    glm::vec3 velVec = ourActor->getVel();
    glm::vec2 xzVec = glm::vec2(velVec.x, velVec.z);
    
    // apply ground friction
    if (xzVec != glm::vec2(0.0,0.0) && xzVec.length() > 0.05f) {
        float heightMin = ourActor->getWorld().getHeightAt(glm::vec2(ourActor->getPos().x, ourActor->getPos().z)); 
        
        if (abs(ourActor->getPos().y - heightMin) < 0.1f) {
        ourActor->setVel(glm::vec3(xzVec.x * 0.9, velVec.y, xzVec.y * 0.9));
        } else { 
            ourActor->setVel(glm::vec3(xzVec.x * 0.99, velVec.y, xzVec.y * 0.99));
        }
        
    } else {
        ourActor->setVel(glm::vec3(0.0, velVec.y, 0.0));
    }
    
    glm::vec3 velPostXZFriction = ourActor->getVel();
    if (abs(velVec.y) > 0.005f) {
        ourActor->setVel(glm::vec3(velPostXZFriction.x, velVec.y * 0.99, velPostXZFriction.z));
    } else {
        ourActor->setVel(glm::vec3(velPostXZFriction.x, 0.0, velPostXZFriction.z));
    }
}
 
void PhysicsComponent::handleGravity(Actor& ourActor) {
    float heightMin = ourActor.getWorld().getHeightAt(glm::vec2(ourActor.getPos().x, ourActor.getPos().z));
    switch(ourActor.getState()) { 
        case STATE_PARALYZED:   
            break;
        case STATE_IDLE:
            
            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin); 
            } else {
                ourActor.accelerate(glm::vec3(0.0,-0.3f*glfwGetTime(),0.0));
            }
            break; 
        case STATE_FLYING:

            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
            } else {
                ourActor.accelerate(glm::vec3(0,-0.3f*glfwGetTime(),0));
            }
            break;
        case STATE_JUMPING:

            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
                ourActor.setState(STATE_IDLE); 
            } else {
                ourActor.accelerate(glm::vec3(0,-0.3f*glfwGetTime(),0));
            }
            break;
    }
}
