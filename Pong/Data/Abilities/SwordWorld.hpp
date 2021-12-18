//
//  SwordWorld.hpp
//  Pong
//
//  Created by Joseph Gu on 12/17/21.
//

#ifndef SwordWorld_hpp
#define SwordWorld_hpp

#include <stdio.h>
#include "Ability.hpp"

class ParticleSystem;

class SwordWorld : public Ability {
private:
    std::weak_ptr<ParticleSystem> swordsRef;
public:
    using Ability::Ability;
    ~SwordWorld();
    
    void call() override;
    
    void tick() override;
    
};
#endif /* SwordWorld_hpp */
