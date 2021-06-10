//
//  Camera.hpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Actor.hpp"
#include <glm/glm.hpp>

class Camera {
    friend class Renderer;
    glm::vec3 posVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    float pitch, yaw, roll;
protected:
    Actor* actor;
public:
    glm::vec3 dirVec;
    Camera();
    Camera(Actor* actor);
    ~Camera();
    void setActor(Actor* actor_);
    float getX();
    float getY();
    float getZ();
    float getPitch();
    float getYaw();
    float getRoll();
    void incPitch(float inc);
    void incYaw(float inc);
    void incRoll(float inc);
    void updateVecs();
    void orientActor();
    void tick();
};

#endif /* Camera_hpp */
