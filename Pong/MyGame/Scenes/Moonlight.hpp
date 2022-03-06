//
//  Moonlight.hpp
//  Pong
//
//  Created by Joseph Gu on 2/12/22.
//

#ifndef Moonlight_hpp
#define Moonlight_hpp

#include <stdio.h>
#include "Script.hpp"
   
class Moonlight : public Script {
public:
    Moonlight(World* world, bool completed);
    virtual void act() override;
}; 

#endif /* Moonlight_hpp */ 
