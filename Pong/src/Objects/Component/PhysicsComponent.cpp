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

PhysicsComponent::PhysicsComponent(bool gravityOn_) {
    gravityOn = gravityOn_;
    Component::type = PHYSICS;
} 

void PhysicsComponent::tick(Actor& actor, World& world) {
    if (actor.getState() == STATE_NORMAL)
    actor.translatePos(actor.getVel());
    handleGravity(actor);
    actor.accelerate(-0.001f*actor.getVel());
    if (glm::vec2(actor.getVel().x,actor.getVel().z).length() < 0.001) {
        actor.setVel(glm::vec3(0,actor.getVel().y,0));
    }
}

void PhysicsComponent::handleGravity(Actor& actor) {
    switch(actor.getState()) {
        case STATE_PARALYZED:
            break;
        case STATE_NORMAL:
            actor.accelerate(glm::vec3(0,-0.1f*glfwGetTime(),0));
            
            if (actor.getPos().y <= 0.0f) {
                actor.setVel(glm::vec3(actor.getVel().x,0,actor.getVel().z));
                actor.setPosY(0.0f);
            }
            break;
        case STATE_FLYING:
            actor.accelerate(glm::vec3(0,-0.1f*glfwGetTime(),0));
            if (actor.getPos().y <= 0.0f) {
                actor.setVel(glm::vec3(actor.getVel().x,0,actor.getVel().z));
                actor.setPosY(0.0f);
            }
            break;
    }
}
