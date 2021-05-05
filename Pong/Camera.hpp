//
//  Camera.hpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

class Camera {
    float x, y, z;
    float pitch, yaw, roll;
public:
    Camera();
    ~Camera();
    void incX(float inc);
    void incY(float inc);
    void incZ(float inc);
    float getX();
    float getY();
    float getZ();
    float getPitch();
    float getYaw();
    float getRoll();
    void incPitch(float inc);
    void incYaw(float inc);
    void incRoll(float inc);
};

#endif /* Camera_hpp */
