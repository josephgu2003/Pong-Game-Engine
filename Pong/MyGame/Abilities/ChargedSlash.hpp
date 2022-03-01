//
//  ChargedSlash.hpp
//  Pong
//
//  Created by Joseph Gu on 1/4/22.
//

#ifndef ChargedSlash_hpp
#define ChargedSlash_hpp

#include <stdio.h>
#include "Ability.hpp"
#include "Prop.hpp"
#include "Particle.hpp"

class ChargedSlash : public Ability {
private:
    std::weak_ptr<Prop> slashRef;
    std::weak_ptr<ParticleSystem> partRef;

public:
    using Ability::Ability; 
    
    ~ChargedSlash();
    void call() override;
    
    void tick() override;
    
};

#endif /* ChargedSlash_hpp */
