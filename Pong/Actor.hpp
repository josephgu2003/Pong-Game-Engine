//
//  Actor.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef Actor_hpp
#define Actor_hpp
#define SPEED 0.01

#include <stdio.h>
#include <string>

class Actor {
    float x, y, z;
    float speed;
    float angle;
    float vertices [108] = {0.0};
public:
    Actor();
    ~Actor();
    void init();
    void tick();
    float* getVertices();
    float getX();
    float getY();
    float getZ();
    int getVerticesCount();
};
#endif /* Actor_hpp */
