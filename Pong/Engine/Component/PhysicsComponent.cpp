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
      
 /**   if (ourActor->getVel() != glm::vec3(0.0) && ourActor->getVel().length() > 0.001) {
        glm::vec3 friction = -0.0001f*glm::normalize(ourActor->getVel());
        ourActor->accelerate(friction);
    } else {
        ourActor->setVel(glm::vec3(0.0,0.0,0.0));
    } **/
    if (ourActor->getVel() != glm::vec3(0.0,0.0,0.0) && ourActor->getVel().length() > 0.05f) {
        ourActor->setVel(ourActor->getVel() * 0.97f);
    } else { 
        ourActor->setVel(glm::vec3(0.0, 0.0, 0.0));
    }
}
 
void PhysicsComponent::handleGravity(Actor& ourActor) {
    float heightMin = ourActor.getWorld().getHeightAt(glm::vec2(ourActor.getPos().x, ourActor.getPos().z));
    switch(ourActor.getState()) { 
        case STATE_PARALYZED:   
            break;
        case STATE_IDLE:
            ourActor.accelerate(glm::vec3(0.0,-0.3f*glfwGetTime(),0.0));
            
            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin); 
            }
            break;
        case STATE_FLYING:
            ourActor.accelerate(glm::vec3(0,-0.3f*glfwGetTime(),0));
            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
            }
            break;
        case STATE_JUMPING:
            ourActor.accelerate(glm::vec3(0,-0.3f*glfwGetTime(),0));
            if (ourActor.getPos().y + ourActor.getVel().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
                ourActor.setState(STATE_IDLE); 
            } 
            break;
    }
}
