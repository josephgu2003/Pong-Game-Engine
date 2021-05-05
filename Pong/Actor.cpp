//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"

void Actor::init() {
    x = 0;
    y = 0;
    z = 0;
    speed = SPEED;
    angle = 1;
    float verticesCopy [] = {
        0.0f,  0.0f,  0.0f,
        0.25f, 0.0f,  0.0f,
        0.0f, 0.25f, 0.0f,
        
        0.0f, 0.25f, 0.0f,
        0.25f, 0.f, 0.0f,
        0.25f, 0.25f, 0.0f,
        
        0.0f,  0.0f,  0.25f,
        0.25f, 0.0f,  0.25f,
        0.0f, 0.25f, 0.25f,
        
        0.0f, 0.25f, 0.25f,
        0.25f, 0.f, 0.25f,
        0.25f, 0.25f, 0.25f,
        //
        0.0f,  0.25f,  0.0f,
        0.25f, 0.25f,  0.0f,
        0.0f, 0.25f, 0.25f,
        
        0.25f, 0.25f, 0.0f,
        0.25f, 0.25f, 0.25f,
        0.0f, 0.25f, 0.25f,
        
        0.0f,  0.0f,  0.0f,
        0.25f, 0.0f,  0.0f,
        0.0f, 0.0f, 0.25f,
        
        0.25f, 0.0f, 0.0f,
        0.25f, 0.0f, 0.25f,
        0.0f, 0.0f, 0.25f,
        //
        0.0f,  0.0f,  0.0f,
        0.0f, 0.25f,  0.0f,
        0.0f, 0.25f, 0.25f,
        
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.25f,
        0.0f, 0.25f, 0.25f,
        
        0.25f,  0.0f,  0.0f,
        0.25f, 0.25f,  0.0f,
        0.25f, 0.25f, 0.25f,
        
        0.25f, 0.0f, 0.0f,
        0.25f, 0.0f, 0.25f,
        0.25f, 0.25f, 0.25f
    };
    memcpy(vertices, verticesCopy, sizeof(verticesCopy));
}

Actor::Actor() {
    
}

Actor::~Actor() {
    
}

void Actor::tick () {
    x += SPEED * cos(angle);
    y += SPEED * sin(angle);
}

float* Actor::getVertices() {
    return vertices;
}

float Actor::getX() {
    return x;
}

float Actor::getY() {
    return y;
}

float Actor::getZ() {
    return z;
}

int Actor::getVerticesCount() {
    return sizeof(vertices)/sizeof(vertices[0]);
}
