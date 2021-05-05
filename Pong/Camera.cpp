//
//  Camera.cpp
//  Pong
//
//  Created by Joseph Gu on 5/5/21.
//

#include "Camera.hpp"

Camera::Camera() {
    x=0.0f;
    y=0.0f;
    z=0.0f;
    pitch=0.0f;
    yaw=-90.0f;
    roll=0.0f;
}
Camera::~Camera() {
    
}
void Camera::incX(float inc) {
    x += inc;
}
void Camera::incY(float inc) {
    y += inc;
}
void Camera::incZ(float inc) {
    z += inc;
}
float Camera::getX() {
    return x;
}
float Camera::getY() {
    return y;
}
float Camera::getZ() {
    return z;
}
float Camera::getPitch() {
    return pitch;
}
float Camera::getYaw() {
    return yaw;
}
float Camera::getRoll() {
    return roll;
}
void Camera::incPitch(float inc) {
    pitch += inc;
}
void Camera::incYaw(float inc) {
    yaw += inc;
}
void Camera::incRoll(float inc) {
    roll += inc;
}
