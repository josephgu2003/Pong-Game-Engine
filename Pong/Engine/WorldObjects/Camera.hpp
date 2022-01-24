//
//  Camera.hpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include "Positionable.hpp"

class Camera : public Positionable {
    friend class Renderer; 
protected:
    Positionable* actor = NULL;
    bool lockOnActor;
    glm::vec3 newRotation;
    bool needRotate;
     
    //states : lock on actor, transition state free floating
public:
    Camera();
    ~Camera();
    void setActor(Positionable* actor_);
    void unlockFromActor();
    void unlockFromActor(glm::vec3 pos, glm::vec3 dir);
    void lockOntoActor();
    void updateVecs();
    void orientActor();
    void tick();
}; 
 
#endif /* Camera_hpp */
