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
    void GramSchmidtAndNormalizeOrientations();

    glm::vec3 posVec;
    glm::vec3 offsetAngles;
    glm::vec3 dirVec;
    glm::vec3 eulerAngles;
    glm::vec3 upVec;
    glm::vec3 rightVec;
public:
    Positionable();


    float getYaw();
    
    glm::vec3 getDir();
    glm::vec3 getPos();
    void setPos(glm::vec3 pos_);
    void translatePos(const glm::vec3& translate);
    void setPosY(float y_);
    virtual  void posDir(float speed);
    virtual void posRight(float speed);
    
    void orient(float yaw);
    void turnTowards(const glm::vec3& newDir);
    void turnTowards(Positionable* p);
    virtual void rotate(glm::vec3 eulers);
    void randomPosAround(glm::vec3 pivot);
    void offsetOrientationVectors(glm::vec3 eulers);

    float getDistanceTo(Positionable* b);
};

#endif /* Positionable_hpp */
