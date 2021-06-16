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
#include "World.hpp"
#include "Actor.hpp"

class FallingLetters: public Ability{
    ComplexParticles* letters = new ComplexParticles;
  //  ParticleEffect* flowers = new ParticleEffect;
public:
    FallingLetters(World* world_, Actor* actor_, float duration_);
    virtual void call(void* pointer);
    virtual void tick();
};
#endif /* FallingLetters_hpp */
