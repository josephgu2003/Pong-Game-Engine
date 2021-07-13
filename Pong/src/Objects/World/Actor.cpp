//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"
#include "World.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include "AssetManager.hpp"
#include "Particle.hpp"
#include "Ability.hpp"
#include "PhysicsComponent.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define JUMP_SPEED 0.05f

Actor::Actor() {
    state = STATE_NORMAL;
        posVec.x = 0.0f;
        posVec.y = 0.0f;
        posVec.z = 0.0f;
        dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
        velVec = glm::vec3(0.0f, 0.0f, 0.0f);
        rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    eulerAngles = glm::vec3(0,-90,0);
    distribution = std::uniform_int_distribution<int>(1,1000);
}

Actor::~Actor() {

}

void Actor::orient(float yaw_) {
    eulerAngles.y = yaw_;
    dirVec.x = std::cos(glm::radians(eulerAngles.y));
    dirVec.z = std::sin(glm::radians(eulerAngles.y));
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    dirVec = glm::normalize(dirVec);
    rightVec = glm::normalize(rightVec);
}

void Actor::turnTowards(glm::vec3 newDir) {
    newDir = glm::normalize(newDir);
    float length = glm::length(newDir);
   // float dRoll = (180/3.14159)*acos(glm::dot(glm::vec3(dirVec.x,dirVec.y,0),glm::vec3(newDir.x,newDir.y,0)));
   // float dPitch =  (180.0f/3.14159)*acos(glm::dot(  glm::normalize(glm::vec3(0,newDir.y,newDir.z)),glm::normalize(glm::vec3(0,dirVec.y,dirVec.z))));
    
  //  float dYaw =  (180.0f/3.14159)*acos(glm::dot(glm::normalize(glm::vec3(newDir.x,0,newDir.z)),glm::normalize(glm::vec3(dirVec.x,0,dirVec.z))));
    float dYaw = (180.0f/3.14159)* glm::orientedAngle(glm::normalize(glm::vec2(newDir.x,newDir.z)), glm::normalize(glm::vec2(dirVec.x,dirVec.z)));
  //  float dPitch = (180.0f/3.14159)* glm::orientedAngle(glm::normalize(glm::vec2(newDir.x,newDir.z)), glm::normalize(glm::vec2(dirVec.x,dirVec.z)));
    eulerAngles -= glm::vec3(0,dYaw,0);
    if (eulerAngles.x < -45.0f) {
        eulerAngles.x = -45.0f;
    }
 //   dirVec.x = std::cos(glm::radians(eulerAngles.y)) * std::cos(glm::radians(eulerAngles.x));
   // dirVec.y = std::sin(glm::radians(eulerAngles.x));
    //dirVec.z = std::sin(glm::radians(eulerAngles.y)) * std::cos(glm::radians(eulerAngles.x));
    dirVec = newDir;
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    rightVec = glm::normalize(rightVec);
}

World& Actor::getWorld() {
    return *world;
}


void Actor::tick() {
    if (components.size() > 0) {
        for (int i = 0; i < components.size(); i++) {
            components.at(i)->tick(*this, *world);
        }
    }
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, posVec);
    modelMat = glm::translate(modelMat, glm::vec3(0,-0.1,0));
    glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(90.0f-eulerAngles.y),glm::radians(eulerAngles.z));
    glm::quat MyQuaternion = glm::quat(rotations);
    
glm::mat4 RotationMatrix = toMat4(MyQuaternion);
    modelMat = modelMat * RotationMatrix;
    
    shader->setMat4("modelMat", modelMat);
}

void Actor::setPos(glm::vec3 pos_) {
    posVec = pos_;
}

void Actor::translatePos(glm::vec3 translate) {
    posVec += translate;
}

void Actor::setPosY(float y_) {
    posVec.y = y_;
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

void Actor::randomPosAround(glm::vec3 pivot) {
    int j = (distribution(generator)%360);
}

// Key by key, I used to sit here to tap out the image of a better world beyond these walls
// I know our room was always small, but it was only because of the confinement that the music was able to focus into our very bones.
// But yesterday I opened the windows and saw a bigger world right there. It's a snowy place, but also a beautiful place. And I saw us, under the soft sunset on that snowy plain.
// I used to think this piano was a key. But is it really a key, or is it a prison?
// I will count to 3. If you don't stop me, I will set fire to this piano and we will go.




void Actor::jump() {
    if (state != STATE_PARALYZED) {
    velVec.y = JUMP_SPEED;
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

float Actor::getYaw() {
    return eulerAngles.y;
}

glm::vec3 Actor::getPos() {
    return posVec;
}

void Actor::init() {
    model = loadModels("Resources/Models/journey6.obj");
   // model = loadModels(MOD_JUGGERNAUT);
    std::vector<GLuint> newMaps_ = {loadTexture("Resources/Models/textures/lambert1_baseColor.png")};
    std::vector<GLuint> newMaps = {loadTexture("Resources/Models/tmpugfolmqr")};
    for (int i = 0; i<model->getMeshes()->size(); i ++) {
        model->setMeshTexture(i, DIFFUSE, newMaps_, newMaps);
    }
    shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader->ID, "ViewProj");
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    extern GLuint uboLights;
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    GLuint lights  = glGetUniformBlockIndex(shader->ID, "Lights");
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
