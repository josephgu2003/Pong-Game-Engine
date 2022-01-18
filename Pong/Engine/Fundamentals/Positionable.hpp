//
//  Positionable.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef Positionable_hpp
#define Positionable_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Subject.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//citing : https://gamedev.stackexchange.com/questions/175006/storing-transformations-in-game-objects-glm-c
constexpr glm::vec3 UPVEC = glm::vec3(0,1,0);
constexpr glm::vec3 DIRVEC = glm::vec3(0,0,1);
constexpr glm::vec3 RIGHTVEC = glm::vec3(1,0,0);

enum DirectionVector {
    DIRECTION,
    UP,
    RIGHTAXIS
};

class Positionable : public Subject {
protected:
    glm::mat4 translationMatrix; 
    glm::quat rotationMatrix;
    glm::quat bakedRotationMatrix;
    glm::mat4 scalingMatrix;
    glm::mat4 modelMatrix;
    bool usingBakedRotations;
    bool updateModelMatrix;
private:
    inline void rotateMatrix(glm::quat& someRotation, glm::vec3& eulers) {
        glm::vec3 eulers_ = glm::vec3(glm::radians(eulers.x), glm::radians(eulers.y), glm::radians(eulers.z));
        glm::quat rotationQuat(eulers_);
    //    glm::mat4 rotationMat = glm::toMat4(rotationQuat);
        someRotation = rotationQuat * someRotation;
        updateModelMatrix = true;
    }
public:
    Positionable();
    
    inline void recalcModelMatrix() {
        if (usingBakedRotations) {
        modelMatrix = translationMatrix * scalingMatrix * glm::toMat4(rotationMatrix)*glm::toMat4(bakedRotationMatrix);
        } else {
            modelMatrix = translationMatrix * scalingMatrix * glm::toMat4(rotationMatrix);
        }
        updateModelMatrix = false; 
    }
    
    inline const glm::mat4& getModelMatrix() {
        if (updateModelMatrix) recalcModelMatrix();
        return modelMatrix;
    } 
    
     glm::vec3 getPos() const {
        glm::vec3 posVec = glm::vec3(translationMatrix[3][0],translationMatrix[3][1],translationMatrix[3][2]);
        return posVec; 
    }
     
     glm::vec3 getDir() {
      //  glm::vec3 dirVec = glm::normalize(glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]));
         glm::vec3 dirVec = glm::vec3(glm::toMat4(rotationMatrix)*glm::vec4(DIRVEC,1));
        return dirVec;
    }

     glm::vec3 getUp() {
         glm::vec3 upVec = glm::vec3(glm::toMat4(rotationMatrix)*glm::vec4(UPVEC,1));
        //glm::vec3 upVec = glm::normalize(glm::vec3(rotationMatrix[0][1], rotationMatrix[1][1], rotationMatrix[2][1]));
        return upVec; 
    }
 
     glm::vec3 getRight() {
         glm::vec3 rightVec = glm::vec3(glm::toMat4(rotationMatrix)*glm::vec4(RIGHTVEC,1));
      //  glm::vec3 rightVec = glm::normalize(glm::vec3(rotationMatrix[0][0], rotationMatrix[1][0], rotationMatrix[2][0]));
        return rightVec;
    }
    
    float getYaw();
    void setPos(glm::vec3 pos_); 
    void translatePos(const glm::vec3& translate);
    void setPosY(float y_);
    
    virtual void posDir(float speed);
    virtual void posRight(float speed);
    
    void orientYawTo(const glm::vec3& newDir);
    void orientYawTo(Positionable* p);
    void lookAt(const glm::vec3& newDir);
    
    virtual void rotate(glm::vec3 eulers);
    void rotateEuler(float degrees, DirectionVector axis);
    void bakeRotation(glm::vec3 eulers);
    void randomPosAround(glm::vec3 pivot);

    float getDistanceTo(Positionable* b);
    float getDistanceTo(glm::vec3 p);
};

#endif /* Positionable_hpp */
