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

class Positionable {
protected:
    glm::vec3 posVec;
public:
    glm::vec3 dirVec;
    glm::vec3 eulerAngles;
    float height = 0.0f;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    glm::vec3 camFocus = glm::vec3(0,0.5,0);

    float getYaw();
    
    glm::vec3 getPos();
    void setPos(glm::vec3 pos_);
    void translatePos(const glm::vec3& translate);
    void setPosY(float y_);
    virtual  void posDir(float speed);
    virtual void posRight(float speed);
    
    void orient(float yaw);
    void turnTowards(const glm::vec3& newDir);
    void rotate(glm::vec3 eulers);
    void randomPosAround(glm::vec3 pivot);


    float getDistanceTo(Positionable* b);
};

#endif /* Positionable_hpp */
