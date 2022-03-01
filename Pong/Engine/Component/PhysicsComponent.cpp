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
    ourActor->accelerate(-0.001f*ourActor->getVel());
    if (glm::vec2(ourActor->getVel().x,ourActor->getVel().z).length() < 0.001) {
        ourActor->setVel(glm::vec3(0,ourActor->getVel().y,0));
    }
}  
 
void PhysicsComponent::handleGravity(Actor& ourActor) {
    float heightMin = ourActor.getWorld().getHeightAt(glm::vec2(ourActor.getPos().x, ourActor.getPos().z));
    switch(ourActor.getState()) { 
        case STATE_PARALYZED:   
            break;
        case STATE_IDLE:  
            ourActor.accelerate(glm::vec3(0,-0.2f*glfwGetTime(),0));
            
            if (ourActor.getPos().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin); 
            }
            break;
        case STATE_FLYING:
            ourActor.accelerate(glm::vec3(0,-0.2f*glfwGetTime(),0));
            if (ourActor.getPos().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
            }
            break;
        case STATE_JUMPING:
            ourActor.accelerate(glm::vec3(0,-0.2f*glfwGetTime(),0));
            if (ourActor.getPos().y < heightMin) {
                ourActor.setVel(glm::vec3(ourActor.getVel().x,0,ourActor.getVel().z));
                ourActor.setPosY(heightMin);
                ourActor.setState(STATE_IDLE); 
            } 
            break;
    }
}
