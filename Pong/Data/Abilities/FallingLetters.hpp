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
#include "RunicLetters.hpp"
#include <memory>

class FallingLetters: public Ability{
private:
    RunicLetters* letters = NULL;
    Force* force = NULL;
    Force* up = NULL;
public:
    FallingLetters(World* world_, Actor* actor_, float duration_);
    ~FallingLetters();
    virtual void call();
    virtual void tick();
};
#endif /* FallingLetters_hpp */
