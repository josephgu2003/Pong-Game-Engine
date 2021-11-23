//
//  ScriptWakeUp.hpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#ifndef ScriptWakeUp_hpp
#define ScriptWakeUp_hpp

#include <stdio.h>
#include "Script.hpp"
#include "ParticleFactory.hpp"
#include "Particle.hpp"

class ScriptWakeUp : public Script {
private:
    ParticleFactory pf;
    std::shared_ptr<ParticleSystem> snow;
public:
    ScriptWakeUp(World* world);
    virtual void act() override;
};


#endif /* ScriptWakeUp_hpp */
