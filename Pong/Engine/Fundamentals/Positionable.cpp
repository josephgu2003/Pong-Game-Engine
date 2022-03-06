//
//  Positionable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "Positionable.hpp"
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>


Positionable::Positionable() {
    translationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::mat4(1.0f);
    scalingMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    bakedRotationMatrix = glm::mat4(1.0f);
    updateModelMatrix = false;
    usingBakedRotations = false; 
} 

float Positionable::getYaw() {
    float yaw = glm::degrees(glm::orientedAngle(getDir(), glm::vec3(0,0,1), glm::vec3(0,1,0)));
    return yaw; 
}

void Positionable::orientYawTo(Positionable* p) {
    orientYawTo(p->getPos()-getPos());
}

void Positionable::lookAt(const glm::vec3& newDir_) {
    assert(glm::vec3(newDir_.x, abs(newDir_.y), newDir_.z) != glm::vec3(0,1,0));
    orientYawTo(newDir_);
   // float pitch = glm::degrees(glm::orientedAngle(glm::normalize(glm::vec2(newDir_.x, newDir_.z)), glm::normalize(glm::vec2(getDir().x, getDir().z))));
 //   rotate(glm::vec3(0.0f,yaw,0.0f));
    
     
   /*8 glm::vec3 newDir = glm::normalize(glm::vec3(newDir_.x, newDir_.y, newDir_.z));
    rotationMatrix[0][2] = newDir.x;
    rotationMatrix[1][2] = newDir.y; 
    rotationMatrix[2][2] = newDir.z;
    
    glm::vec3 newRight = glm::normalize(glm::cross(glm::normalize(newDir), glm::vec3(0,1,0)));
    rotationMatrix[0][0] = newRight.x;
    rotationMatrix[1][0] = newRight.y;
    rotationMatrix[2][0] = newRight.z;
    
    glm::vec3 newUp = glm::normalize(glm::cross(glm::normalize(newDir), newRight));
    rotationMatrix[0][1] = newUp.x;
    rotationMatrix[1][1] = newUp.y;
    rotationMatrix[2][1] = newUp.z;**/
    updateModelMatrix = true;
}

void Positionable::rotateEuler(float degrees, DirectionVector axis) {
    auto makeRotation = [&] (glm::vec3& axis) {
        glm::quat rotationQuat = glm::angleAxis(glm::radians(degrees), axis);
        rotationMatrix = rotationQuat * rotationMatrix;
        updateModelMatrix = true;
    };
    switch (axis) {
        case DIRECTION: {
            glm::vec3 axis = getDir();
            makeRotation(axis);
            break;
        }
        case UP: {
            glm::vec3 axis = getUp();
            makeRotation(axis);
            break;
        }
        case RIGHTAXIS: {
            glm::vec3 axis = getRight();
            makeRotation(axis);
            break;
        } 
        default:
            break;
    }
}

void Positionable::orientYawTo(const glm::vec3& newDir_) {
    assert(glm::vec3(newDir_.x, abs(newDir_.y), newDir_.z) != glm::vec3(0,1,0));
    float yaw = glm::degrees(glm::orientedAngle(glm::normalize(glm::vec2(newDir_.x, newDir_.z)), glm::normalize(glm::vec2(getDir().x, getDir().z))));
    rotate(glm::vec3(0.0f,yaw,0.0f));  
          
  /**  glm::vec3 oldDir = getDir();
    
    float baseLength = sqrt(1.0f-pow(oldDir.y,2)); // what should base length be
    float actualLength = sqrt(pow(newDir_.x, 2) + pow(newDir_.z,2)); 
    float scaleFactor = baseLength/actualLength;

    rotationMatrix = glm::mat4(1.0f);
    glm::vec3 newDir = glm::normalize(glm::vec3(scaleFactor*newDir_.x, oldDir.y, scaleFactor*newDir_.z));
    rotationMatrix[0][2] = newDir.x;
    rotationMatrix[1][2] = newDir.y; 
    rotationMatrix[2][2] = newDir.z;
    
    glm::vec3 newRight = glm::normalize(glm::cross(glm::vec3(0,1,0), newDir));
    rotationMatrix[0][0] = newRight.x;
    rotationMatrix[1][0] = newRight.y; 
    rotationMatrix[2][0] = newRight.z;
      
    glm::vec3 newUp = glm::normalize(glm::cross(newDir, newRight));
    rotationMatrix[0][1] = newUp.x;
    rotationMatrix[1][1] = newUp.y;
    rotationMatrix[2][1] = newUp.z;**/
    updateModelMatrix = true;
}

void Positionable::bakeRotation(glm::vec3 eulers) {
    rotateMatrix(bakedRotationMatrix, eulers);
    usingBakedRotations = true;
}

void Positionable::setPos(glm::vec3 pos_) {
    translationMatrix[3][0] = pos_.x;
    translationMatrix[3][1] = pos_.y;
    translationMatrix[3][2] = pos_.z;
    updateModelMatrix = true;
    Subject::notifyAll(SUBJ_POS_CHANGED);
}

void Positionable::translatePos(const glm::vec3& translate) {
    translationMatrix = glm::translate(translationMatrix, translate);
    notifyAll(SUBJ_POS_CHANGED);
    updateModelMatrix = true;
}

void Positionable::setPosY(float y_) {
    translationMatrix[3][1] = y_;
    notifyAll(SUBJ_POS_CHANGED);
    updateModelMatrix = true; 
}

void Positionable::posDir(float speed) {
    glm::vec3 dir = speed * getDir();
    translationMatrix = glm::translate(translationMatrix, dir);
    notifyAll(SUBJ_POS_CHANGED);
    updateModelMatrix = true;
}
 
void Positionable::posRight(float speed) {
    glm::vec3 right = speed * getRight();
    translationMatrix = glm::translate(translationMatrix, right);
    notifyAll(SUBJ_POS_CHANGED);
    updateModelMatrix = true;
}

void Positionable::randomPosAround(glm::vec3 pivot) {

}

void Positionable::rotate(glm::vec3 eulers) {
    rotateMatrix(rotationMatrix, eulers);
}

  
float Positionable::getDistanceTo(Positionable* b) {
    float d = glm::length(getPos() - b->getPos());
    return d;
}
  
float Positionable::getDistanceTo(glm::vec3 p) {
    float d = glm::length(getPos() - p);
    return d;
}

void Positionable::setScale(float scale) {
    scalingMatrix = glm::mat4(1.0f);
    scalingMatrix = glm::scale(scalingMatrix,glm::vec3(scale,scale,scale));
}

void Positionable::updateTranslation(const glm::mat4& trans) {
    translationMatrix = trans * translationMatrix;
}
void Positionable::updateRotation(const glm::quat& rot) {
    rotationMatrix = rot * rotationMatrix;
}
void Positionable::updateScaling(const glm::mat4& scale_) {
    scalingMatrix = scale_ * scalingMatrix;
}

/**
#include <glm/gtx/vector_angle.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
modelMat = glm::mat4(1.0f);
modelMat = glm::translate(modelMat, posVec);
glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(90.0f-eulerAngles.y),glm::radians(eulerAngles.z));
glm::quat MyQuaternion = glm::quat(rotations);

glm::mat4 RotationMatrix = toMat4(MyQuaternion);
modelMat = modelMat * RotationMatrix;
**/
