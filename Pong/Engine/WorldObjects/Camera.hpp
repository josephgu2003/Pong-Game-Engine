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
#include "Watch.hpp"

enum CameraState {
    CAM_FREE,
    CAM_FOLLOW_ACTOR_ALIGNED,
    CAM_FOLLOW_ACTOR_UNALIGNED
};

class Camera : public Positionable {
    friend class Renderer; 
protected:
    Positionable* actor = NULL;
    glm::vec3 newRotation;
    bool needRotate;
    CameraState state;
    
    bool interpolating; // hehehe
    float interpolationDuration;
    Watch watch;
    glm::vec3 oldPos;
    glm::vec3 oldDir;
    //states : lock on actor, transition state free floating
public:
    Camera();
    ~Camera();
    void setActor(Positionable* actor_);
    void unlockFromActor();
    void unlockFromActor(glm::vec3 pos, glm::vec3 dir);
    void lockOntoActor();
    void setState(CameraState state);
    void setStateAndInterpolate(CameraState state, float timeTo);
    void updateVecs();
    void tick();
}; 
 
#endif /* Camera_hpp */
