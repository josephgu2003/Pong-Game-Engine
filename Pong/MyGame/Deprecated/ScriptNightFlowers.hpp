//
//  ScriptNightFlowers.hpp
//  Pong
//
//  Created by Joseph Gu on 1/16/22.
//

#ifndef ScriptNightFlowers_hpp
#define ScriptNightFlowers_hpp

#include <stdio.h>
#include "Script.hpp"
 
class ScriptNightFlowers : public Script {
public:
    ScriptNightFlowers(World* world, bool completed);
    virtual void act() override;
};

#endif /* ScriptNightFlowers_hpp */
