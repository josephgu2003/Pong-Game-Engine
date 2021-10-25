//
//  FallingLetters.hpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#ifndef FallingLetters_hpp
#define FallingLetters_hpp

#include <stdio.h>
#include "Ability.hpp"
#include "Particle.hpp"
#include <memory>

class World;
class Actor;

class FallingLetters: public Ability{
private:
    ParticleSystem* letters = NULL;
public:
    FallingLetters(World* world_, Actor* actor_, float duration_);
    ~FallingLetters();
    virtual void call();
    virtual void tick();
};
#endif /* FallingLetters_hpp */
