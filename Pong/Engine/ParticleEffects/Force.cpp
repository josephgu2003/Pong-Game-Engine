//
//  Force.cpp
//  Pong
//
//  Created by Joseph Gu on 7/27/21.
//

#include "Force.hpp"
#include "Actor.hpp"

Force::Force() {
    dir = glm::vec3(0,1,0); 
    magnitude = 1.0f;
    fType = FORCE_DIRECT;
    fGradient = FORCE_UNIFORM;
}

Force::Force(glm::vec3 pos_, float magnitude_) {
    init(pos_, magnitude_);
}
void Force::init(glm::vec3 pos_, float magnitude_) {
    setPos(pos_);
    dir = glm::vec3(-1,0,0);
    magnitude = magnitude_;
}
void Force::affectVelAt(glm::vec3& point, glm::vec3& vel) {
    glm::vec3 force;
    float distance = glm::length(point - getPos());
    switch (fType){
        case FORCE_DIRECT:
           force = glm::normalize(dir);
            break;
        case FORCE_SUCTION:
            force = glm::normalize(getPos() - point);
            break;
        case FORCE_VORTEX:
            force = (shear)*glm::normalize(getPos() - point)+(1.0f-shear)* glm::normalize(glm::cross(glm::vec3(0,1,0),glm::normalize(getPos() - point)));
            break; 
                 
    } 
    switch (fGradient){  
        case FORCE_UNIFORM: 
            force *= magnitude;
            break;
        case FORCE_LINEAR: {

            float f = (1.0f/(1.0*distance+0.5));
            if (f < 0) f = 0; 
            force *= f * magnitude;
            break;
        }
        case FORCE_QUADRATIC:
            float f2 = (1.0f/(1.0*distance*distance+0.5));
            if (f2 < 0) f2 = 0;
            force *= f2 * magnitude; 
            break;
    }
    vel += force;
}
void Force::configureStraightForce(glm::vec3 dir_,float gradientK_,  ForceGradient g) {
    dir = dir_;
    fGradient = g;
    gradientK = gradientK_;
    fType = FORCE_DIRECT;
}
void Force::configureVortexForce(float shear_, float gradientK_, ForceGradient g) {
    fGradient = g;
    gradientK = gradientK_;
    fType = FORCE_VORTEX;
    shear = shear_;
    if (shear > 1.0) shear = 1.0;
    if (shear < 0.0) shear = 0.0;  
}

void Force::configureSuctionForce(float gradientK_, ForceGradient g) {
    gradientK = gradientK_;
    fType = FORCE_SUCTION;
    fGradient = g;
}
void Force::setActor(const std::shared_ptr<Actor>& actor_) {
    actor = actor_;
}

void Force::tick() {
    if (actor.get() != NULL) {
        setPos(actor->getPos());
    }
}


glm::vec3 Force::getUniStraightForce() {
    if (fType == FORCE_DIRECT && fGradient == FORCE_UNIFORM)
        return magnitude * dir;
    return glm::vec3(0.0f); 
}
 
 
