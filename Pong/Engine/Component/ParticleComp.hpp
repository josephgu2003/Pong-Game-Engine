//
//  ParticleComp.hpp
//  Pong
//
//  Created by Joseph Gu on 10/20/21.
//

#ifndef ParticleComp_hpp
#define ParticleComp_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Particle.hpp"

class ParticleComp : public Component<ParticleSystem> {
protected:

public:
    ParticleComp(ParticleSystem& pe);
};

#endif /* ParticleComp_hpp */
