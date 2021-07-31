//
//  Ball.hpp
//  Pong
//
//  Created by Joseph Gu on 5/1/21.
//

#ifndef Ball_hpp
#define Ball_hpp
#include "Actor.hpp"
#include "Particle.hpp"

class Ball: public Actor {
public:
    Ball();
    virtual void tick() override;
    virtual void init(int i) override;
};

#endif /* Ball_hpp */


