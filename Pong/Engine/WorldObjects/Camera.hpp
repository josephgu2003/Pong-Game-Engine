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
#include "Positionable.hpp"

class Camera : public Positionable {
    friend class Renderer;
protected:
    Actor* actor = NULL;
public:
    Camera();
    Camera(Actor* actor);
    ~Camera();
    void setActor(Actor* actor_);
    void updateVecs();
    void orientActor();
    void tick();
}; 

#endif /* Camera_hpp */
