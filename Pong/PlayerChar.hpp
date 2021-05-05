//
//  PlayerChar.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef PlayerChar_hpp
#define PlayerChar_hpp

#include <stdio.h>
class PlayerChar {
    // location x,y speed sx, sy, angle ansgle
    float x, y;
    float speed;
    float angle;
    float vertices [108] = {0.0};
public:
    PlayerChar();
    ~PlayerChar();
    void init();
    void tick();
    float* getVertices();
    float getX();
    float getY();
};
#endif /* PlayerChar_hpp */
