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
    ParticleEffect fiery;
public:
    void loadModel();
    ParticleEffect& getParticleEffect();
};

#endif /* Ball_hpp */


