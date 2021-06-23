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
}

void PhysicsComponent::tick(Actor& actor, World& world) {
    if (actor.state == STATE_NORMAL)
    actor.posVec += actor.velVec;
    handleGravity(actor);
    if (actor.velVec.x > 0) {
        if (actor.velVec.x > glfwGetTime()) {
            actor.velVec.x +=  -glfwGetTime();
        } else {
            actor.velVec.x = 0;
        }
    };
    if (actor.velVec.z > 0)     {
        if (actor.velVec.z > glfwGetTime()) {
            actor.velVec.z +=  -glfwGetTime();
        } else {
            actor.velVec.z = 0;
        }
    }
    if (actor.velVec.x < 0) {
        if (actor.velVec.x <-glfwGetTime()) {
            actor.velVec.x +=  glfwGetTime();
        } else {
            actor.velVec.x = 0;
        }
    }
    if (actor.velVec.z < 0) {
        if (actor.velVec.z <-glfwGetTime()) {
            actor.velVec.z +=  glfwGetTime();
        } else {
            actor.velVec.z = 0;
        }
    }
}

void PhysicsComponent::handleGravity(Actor& actor) {
    switch(actor.state) {
        case STATE_PARALYZED:
            break;
        case STATE_NORMAL:
            actor.velVec.y += -0.1f*glfwGetTime();
            if (actor.posVec.y <= 0.0f) {
                actor.velVec.y = 0;
                actor.posVec.y = 0.0f;
            }
            break;
        case STATE_FLYING:
            actor.velVec.y += -0.01f*glfwGetTime();
            if (actor.posVec.y <= 0.0f) {
                actor.velVec.y = 0;
                actor.posVec.y = 0.0f;
            }
            break;
    }
}
