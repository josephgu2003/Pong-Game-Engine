//
//  Force.hpp
//  Pong
//
//  Created by Joseph Gu on 7/27/21.
//

#ifndef Force_hpp
#define Force_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Actor.hpp"
#include <memory>
enum ForceGradient {
    FORCE_UNIFORM,
    FORCE_LINEAR,
    FORCE_QUADRATIC
};
enum ForceType {
    FORCE_DIRECT,
    FORCE_SUCTION,
    FORCE_VORTEX
};
class Force {
protected:
    glm::vec3 pos;
    glm::vec3 dir;
    float magnitude;
    float gradientK;
    float shear;
    ForceType fType;
    ForceGradient fGradient;
    std::shared_ptr<Actor> actor;
public:
    Force();
    Force(glm::vec3 pos_, float magnitude);
    void init(glm::vec3 pos_, float magnitude);
    void tick();
    glm::vec3 getPos();
    void setActor(Actor* actor);
    void configureStraightForce(glm::vec3 dir, float gradientK_, ForceGradient g);
    void configureVortexForce(float shear, float gradientK_, ForceGradient g);
    void configureSuctionForce(float gradientK_, ForceGradient g);
    void affectVelAt(glm::vec3& point, glm::vec3& pos);
    glm::vec3 getUniStraightForce();
};
#endif /* Force_hpp */
