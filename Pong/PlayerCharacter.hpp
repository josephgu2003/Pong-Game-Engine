//
//  PlayerCharacter.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef PlayerCharacter_hpp
#define PlayerCharacter_hpp

#include <stdio.h>

class PlayerCharacter {
    // location x,y speed sx, sy, angle ansgle
    float x, y;
    float speed;
    float angle;
    float vertices [108] = {0.0};
public:
    PlayerCharacter();
    ~PlayerCharacter();
    void init();
    void tick();
    float* getVertices();
    float getX();
    float getY();
};

#endif /* PlayerCharacter_hpp */
