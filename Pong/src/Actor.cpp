//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"
#define JUMP_SPEED 0.05f

Actor::Actor() {
        posVec.x = 0.0f;
        posVec.y = 0.0f;
        posVec.z = 0.0f;
        dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
        velVec = glm::vec3(0.0f, 0.0f, 0.0f);
        rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
        float verticesCopy [] = {
            -0.125f, -0.125f, -0.125f,
            -0.125f, -0.125f, 0.125f,
            0.125f, -0.125f, 0.125f,
            0.125f, -0.125f, -0.125f,
            -0.125f, 0.125f, -0.125f,
            -0.125f, 0.125f, 0.125f,
            0.125f, 0.125f, 0.125f,
            0.125f, 0.125f, -0.125f
        };
        memcpy(vertices, verticesCopy, sizeof(verticesCopy));
        GLuint indicesCopy [] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 1, 5,
            5, 4, 0,
            3, 2, 6,
            6, 7, 3,
            1, 2, 5,
            5, 6, 2,
            0, 3, 7,
            7, 4, 0
        };
    memcpy(indices, indicesCopy, sizeof(indicesCopy));
}

Actor::~Actor() {

}

void Actor::updateVecs() { //updates position and velocity
    velVec.y += -0.1f*glfwGetTime();
    posVec += velVec;
    if (posVec.y <= 0.0f) {
        velVec.y = 0;
        posVec.y = 0.0f;
    }
    if (velVec.x > 0) {
        if (velVec.x > glfwGetTime()) {
            velVec.x +=  -glfwGetTime();
        } else {
            velVec.x = 0;
        }
    };
    if (velVec.z > 0)     {
        if (velVec.z > glfwGetTime()) {
            velVec.z +=  -glfwGetTime();
        } else {
            velVec.z = 0;
        }
    }
    if (velVec.x < 0) {
        if (velVec.x <-glfwGetTime()) {
            velVec.x +=  glfwGetTime();
        } else {
            velVec.x = 0;
        }
    }
    if (velVec.z < 0) {
        if (velVec.z <-glfwGetTime()) {
            velVec.z +=  glfwGetTime();
        } else {
            velVec.z = 0;
        }
    }
}

void Actor::orient(float yaw_) {
    yaw = yaw_;
    dirVec.x = std::cos(glm::radians(yaw));
    dirVec.z = std::sin(glm::radians(yaw));
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    dirVec = glm::normalize(dirVec);
    rightVec = glm::normalize(rightVec);
}

void Actor::stop() {
}

void Actor::posDir(float speed) {
    posVec += speed * dirVec;
}
void Actor::posRight(float speed) {
    posVec += speed * rightVec;
}

void Actor::jump() {
    velVec.y = JUMP_SPEED;
}
void Actor::velRight(float speed) {
    velVec += rightVec*speed;
}
void Actor::velDir(float speed) {
    velVec += dirVec*speed;
}

float Actor::getYaw() {
    return yaw;
}
float* Actor::getVertices() {
    return vertices;
}
GLuint* Actor::getIndices() {
    return indices;
}
glm::vec3 Actor::getPos() {
    return posVec;
}

int Actor::getVerticesCount() {
    return sizeof(vertices)/sizeof(vertices[0]);
}

int Actor::getIndicesCount() {
    return sizeof(indices)/sizeof(indices[0]);
}

void Actor::loadModel() {
    model = new Model("/Users/josephgu/Downloads/juggernaut/juggernaut/materials/juggernaut_econ.fbx");
}

